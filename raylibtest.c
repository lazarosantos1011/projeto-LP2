#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WALL_THICKNESS 10

#define PLAYER_SPEED 200
#define ZOMBIE_SPEED 190

typedef struct {
    Vector2 position;
    float radius;
    Color color;
} Entity;

Entity player;
Entity zombie;

bool gameOver = false;
bool restartLevel = false;

Rectangle walls[4];
Rectangle books[4];
int collectedBooks = 0;

typedef struct {
    Rectangle rect;
    Color color;
} Wall;

Wall topWall, bottomWall, leftWall, rightWall;

void InitGame() {
    
    // Inicializar paredes
    topWall.rect = (Rectangle){ 0, 0, SCREEN_WIDTH, WALL_THICKNESS };
    bottomWall.rect = (Rectangle){ 0, SCREEN_HEIGHT - WALL_THICKNESS, SCREEN_WIDTH, WALL_THICKNESS };
    leftWall.rect = (Rectangle){ 0, WALL_THICKNESS, WALL_THICKNESS, SCREEN_HEIGHT - 2 * WALL_THICKNESS };
    rightWall.rect = (Rectangle){ SCREEN_WIDTH - WALL_THICKNESS, WALL_THICKNESS, WALL_THICKNESS, SCREEN_HEIGHT - 2 * WALL_THICKNESS };
    topWall.color = bottomWall.color = leftWall.color = rightWall.color = DARKGRAY;
    
    // Inicializar jogador
    player.position = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    player.radius = 20;
    player.color = RED;
    
    // Inicializar zumbi
    zombie.position = (Vector2){ 100, 100 };
    zombie.radius = 20;
    zombie.color = GREEN;
    
    gameOver = false;
    restartLevel = false;
    
    // Inicializar paredes
    /*walls[0] = (Rectangle){ 0, 0, 20, SCREEN_HEIGHT };
    walls[1] = (Rectangle){ 0, 0, SCREEN_WIDTH, 20 };
    walls[2] = (Rectangle){ SCREEN_WIDTH - 20, 0, 20, SCREEN_HEIGHT };
    walls[3] = (Rectangle){ 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20 };
    */
    
    // Inicializar livros
    books[0] = (Rectangle){ 100, 100, 40, 40 };
    books[1] = (Rectangle){ 600, 100, 40, 40 };
    books[2] = (Rectangle){ 100, 400, 40, 40 };
    books[3] = (Rectangle){ 600, 400, 40, 40 };
    
    collectedBooks = 0;
}

void UpdateGame() {
    if(gameOver && IsKeyPressed(KEY_ENTER)) {
        restartLevel = true;
        return;
    }
    
    if (restartLevel) {
        InitGame();
        restartLevel = false;
        return;
    }
    
    if(gameOver)
        return;
    
    // Atualizar jogador
    if (IsKeyDown(KEY_W) && player.position.y - player.radius > 0)
        player.position.y -= PLAYER_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_S) && player.position.y + player.radius < SCREEN_HEIGHT)
        player.position.y += PLAYER_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_A) && player.position.x - player.radius > 0)
        player.position.x -= PLAYER_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_D) && player.position.x + player.radius < SCREEN_WIDTH)
        player.position.x += PLAYER_SPEED * GetFrameTime();
    
    // Atualizar zumbi
    Vector2 direction = Vector2Subtract(player.position, zombie.position);
    direction = Vector2Normalize(direction);
    zombie.position.x += direction.x * ZOMBIE_SPEED * GetFrameTime();
    zombie.position.y += direction.y * ZOMBIE_SPEED * GetFrameTime();
    
    // Checar colisões com livros
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionCircleRec(player.position, player.radius, books[i])) {
            if (!books[i].width && !books[i].height) continue; // Livro já coletado
            
            collectedBooks++;
            books[i].width = 0;
            books[i].height = 0;
        }
    }
    
    // Checar colisões com paredes
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionCircleRec(player.position, player.radius, walls[i])) {
            // Colisão com parede, fazer tratamento adequado (ex: parar movimento)
        }
    }
    
    // Checar colisões com zumbi
    if (CheckCollisionCircles(player.position, player.radius, zombie.position, zombie.radius)) {
        // Colisão com zumbi, fazer tratamento adequado (ex: fim de jogo)
        gameOver = true;
    }
}

void DrawGame() {
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    // Desenhar paredes
    DrawRectangleRec(topWall.rect, topWall.color);
    DrawRectangleRec(bottomWall.rect, bottomWall.color);
    DrawRectangleRec(leftWall.rect, leftWall.color);
    DrawRectangleRec(rightWall.rect, rightWall.color);
    
    // Desenhar jogador
    DrawCircleV(player.position, player.radius, player.color);
    
    // Desenhar zumbi
    DrawCircleV(zombie.position, zombie.radius, zombie.color);
    
    // Draw game over text
    if (gameOver) {
        DrawText("Game Over", SCREEN_WIDTH / 2 - MeasureText("Game Over", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, RED);
    }
    
    // Desenhar paredes
    /*for (int i = 0; i < 4; i++) {
        DrawRectangleRec(walls[i], DARKGRAY);
    }*/
    
    // Desenhar livros coletados
    for (int i = 0; i < 4; i++) {
        if (books[i].width && books[i].height) {
            DrawRectangleRec(books[i], BLUE);
        }
    }
    
    // Desenhar informações na tela
    DrawText("Livros coletados: ", 10, 10, 20, BLACK);
    DrawText(TextFormat("%d", collectedBooks), 190, 11, 20, BLACK);
    
    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Game");
    SetTargetFPS(60);
    
    InitGame();
    
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    
    CloseWindow();
    
    return 0;
}