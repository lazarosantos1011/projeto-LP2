#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_SPEED 200
#define ZOMBIE_SPEED 150

typedef struct {
    Vector2 position;
    float radius;
    Color color;
    Texture2D texture; // New field for the player sprite texture
} Entity;

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool collected;
} Book;

Entity player;
Entity zombie;
Book books[5];

bool gameOver = false;
bool gameWon = false;
bool restartLevel = false;

int numBooks = 5;
int booksCollected = 0;

void InitGame() {
    // Initialize player
    player.position = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    player.radius = 20;
    player.color = WHITE;

    // Load the player sprite image
    player.texture = LoadTexture("C:/Users/Casa/Pictures/player_sprite.png");

    // Initialize zombie
    zombie.position = (Vector2){ 100, 100 };
    zombie.radius = 20;
    zombie.color = WHITE; // No longer needed for the zombie

    // Load the zombie sprite image
    zombie.texture = LoadTexture("C:/Users/Casa/Pictures/zombie_sprite.png");

    // Load the book sprite image and create book entities
    for (int i = 0; i < numBooks; i++) {
        books[i].position = (Vector2){ GetRandomValue(100, SCREEN_WIDTH - 100), GetRandomValue(100, SCREEN_HEIGHT - 100) };
        books[i].texture = LoadTexture("C:/Users/Casa/Pictures/cBook_sprite.png");
        books[i].collected = false;
    }

    gameOver = false;
    gameWon = false;
    restartLevel = false;

    booksCollected = 0;
}

void UpdateGame() {
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        restartLevel = true;
        return;
    }

    if (restartLevel) {
        InitGame();
        restartLevel = false;
        return;
    }

    if (gameWon && IsKeyPressed(KEY_ENTER)) {
        restartLevel = true;
        return;
    }

    if (gameOver || gameWon)
        return;

    // Update player movement
    Vector2 playerDirection  = { 0 }; // Initialize a direction vector with (0, 0)

    if (IsKeyDown(KEY_W)) playerDirection .y -= 1; // Up
    if (IsKeyDown(KEY_S)) playerDirection .y += 1; // Down
    if (IsKeyDown(KEY_A)) playerDirection .x -= 1; // Left
    if (IsKeyDown(KEY_D)) playerDirection .x += 1; // Right

    // Normalize the direction vector to ensure the player moves at the same speed regardless of the direction
    if (playerDirection .x != 0 || playerDirection .y != 0) {
        float length = sqrtf(playerDirection .x * playerDirection .x + playerDirection .y * playerDirection .y);
        playerDirection .x /= length;
        playerDirection .y /= length;
    }

    // Update player position based on the normalized direction
    player.position.x += playerDirection .x * PLAYER_SPEED * GetFrameTime();
    player.position.y += playerDirection .y * PLAYER_SPEED * GetFrameTime();


    // Update zombie
    Vector2 zombieDirection = Vector2Subtract(player.position, zombie.position);
    zombieDirection = Vector2Normalize(zombieDirection);
    zombie.position.x += zombieDirection.x * ZOMBIE_SPEED * GetFrameTime();
    zombie.position.y += zombieDirection.y * ZOMBIE_SPEED * GetFrameTime();

    // Check collision with zombie
    if (CheckCollisionCircles(player.position, player.radius, zombie.position, zombie.radius)) {
        gameOver = true;
    }

    // Check collision with books
    for (int i = 0; i < numBooks; i++) {
        if (books[i].collected) // Skip books that have been collected already
            continue;
            
        if (CheckCollisionCircles(player.position, player.radius, books[i].position, books[i].texture.width / 2)) {
            books[i].collected = true; // Set the book as collected
            booksCollected++;
        }
    }

    // Check if all books have been collected
    if (booksCollected == numBooks) {
        gameWon = true;
    }
}

void DrawGame() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw player using the loaded texture
    DrawTexture(player.texture, player.position.x - player.texture.width / 2, player.position.y - player.texture.height / 2, WHITE);

    // Draw zombie using the loaded texture
    DrawTexture(zombie.texture, zombie.position.x - zombie.texture.width / 2, zombie.position.y - zombie.texture.height / 2, WHITE);

    // Draw books
    for (int i = 0; i < numBooks; i++) {
        if (!books[i].collected) // Draw only books that have not been collected
            DrawTexture(books[i].texture, books[i].position.x - books[i].texture.width / 2, books[i].position.y - books[i].texture.height / 2, WHITE);
    }
    
    // Draw game over screen
    if (gameOver) {
        DrawText("Game Over", SCREEN_WIDTH / 2 - MeasureText("Game Over", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, RED);
        DrawText("Pressione Enter para tentar novamente ou Esc para sair", SCREEN_WIDTH / 2 - MeasureText("Pressione Enter para tentar novamente ou Esc para sair", 20) / 2, SCREEN_HEIGHT / 2 + 20, 20, DARKGRAY);
    }
    
    // Draw game won screen
    if (gameWon) {
        DrawText("Parabéns!", SCREEN_WIDTH / 2 - MeasureText("Parabéns!", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, GREEN);
        DrawText("Você resgatou todos os livros e derrotou o funcionário da UFPE!", SCREEN_WIDTH / 2 - MeasureText("Você resgatou todos os livros e derrotou o funcionário da UFPE!", 20) / 2, SCREEN_HEIGHT / 2 + 20, 20, DARKGREEN);
        DrawText("Pressione Enter para Reiniciar ou Esc para Sair", SCREEN_WIDTH / 2 - MeasureText("Pressione Enter para Reiniciar ou Esc para Sair", 20) / 2, SCREEN_HEIGHT / 2 + 60, 20, DARKGRAY);
    }

    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aventura Universitária: O Encontro com o Predador");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    // Unload textures when closing the window
    UnloadTexture(player.texture);
    UnloadTexture(zombie.texture);
    for (int i = 0; i < numBooks; i++) {
        if (!books[i].collected)
            UnloadTexture(books[i].texture);
    }

    CloseWindow();

    return 0;
}
