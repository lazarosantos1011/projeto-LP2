#include <raylib.h>
#include <raymath.h>
#include <string.h> // Include string.h for strlen function
#include <stdio.h>  // Include stdio.h for FILE and file related functions
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_SPEED 200
#define ZOMBIE_SPEED 150

#define INITIAL_NUM_BOOKS 5
#define MAX_NUM_BOOKS 20

#define MAX_SCORES 20 // Maximum number of scores to store in the file

// Function prototypes
void SaveScore(const char* playerName, int score);
void LoadScores();
void DrawScores();
void AskPlayerName();

typedef struct {
    char playerName[20]; // Array to store the player's name
    int score;
} ScoreEntry;

ScoreEntry scores[MAX_SCORES]; // Global array to store the scores

Music bgMusic;

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

typedef struct {
    Rectangle rect;
} Wall;

typedef enum {
    STATE_MENU,
    STATE_PLAY,
    STATE_RANKING,
    STATE_CREDITS,
    STATE_DONATIONS
} GameState;


Entity player;
Entity zombie;
Book books[5];
Wall walls[12];

bool gameOver = false;
bool gameWon = false;
bool restartLevel = false;

int numBooks = 5;
int booksCollected = 0;

Texture2D grassTexture;

// Function prototypes
void DrawMenu();
void DrawCredits();
void DrawDonations();

void InitGame() {
    // Initialize player
    player.position = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    player.radius = 20;
    player.color = WHITE;

    // Load the player sprite image
    player.texture = LoadTexture("C:/Users/Casa/Music/Pictures/player_sprite.png");

    // Initialize zombie
    zombie.position = (Vector2){ 100, 100 };
    zombie.radius = 20;
    zombie.color = WHITE; // No longer needed for the zombie

    // Load the zombie sprite image
    zombie.texture = LoadTexture("C:/Users/Casa/Music/Pictures/zombie_sprite.png");
    
    /// Define wall positions
    walls[0].rect = (Rectangle){ 100, 0, 10, SCREEN_HEIGHT }; // Left wall
    walls[1].rect = (Rectangle){ 0, 100, SCREEN_WIDTH, 10 }; // Top wall
    walls[2].rect = (Rectangle){ 100, SCREEN_HEIGHT - 10, SCREEN_WIDTH - 100, 10 }; // Bottom wall
    walls[3].rect = (Rectangle){ SCREEN_WIDTH - 10, 100, 10, SCREEN_HEIGHT - 100 }; // Right wall
    
    // Additional walls for the cross shape
    walls[4].rect = (Rectangle){ 200, 100, 10, 100 }; // Vertical wall for the upper arm of the cross
    walls[5].rect = (Rectangle){ 100, 200, 100, 10 }; // Horizontal wall for the left arm of the cross
    walls[6].rect = (Rectangle){ 200, 300, 10, 100 }; // Vertical wall for the lower arm of the cross
    walls[7].rect = (Rectangle){ 300, 200, 100, 10 }; // Horizontal wall for the right arm of the cross

    // Completing the cross shape
    walls[8].rect = (Rectangle){ 200, 200, 10, 100 }; // Vertical wall to connect the left and right arms
    walls[9].rect = (Rectangle){ 200, 200, 100, 10 }; // Horizontal wall to connect the upper and lower arms
    walls[10].rect = (Rectangle){ 200, 200, 100, 100 }; // Center square
    walls[11].rect = (Rectangle){ 210, 210, 80, 80 }; // Inner square
    
    // Load grass texture
    grassTexture = LoadTexture("C:/Users/Casa/Music/Pictures/grass.png");

    // Load the book sprite image and create book entities

    int numBooks = INITIAL_NUM_BOOKS;

    void AddBooksOnCollect(Book books[]){
        if (numBooks < MAX_NUM_BOOKS) {
            numBooks++;

            int newBookIndex = numBooks - 1;
            books[newBookIndex].position = (Vector2){ GetRandomValue(100, SCREEN_WIDTH - 100), GetRandomValue(100, SCREEN_HEIGHT - 100) };
            books[newBookIndex].texture = LoadTexture("C:/Users/Casa/Music/Pictures/cBook_sprite.png");
            books[newBookIndex].collected = false;
        }
    };
    
   /* for (int i = 0; i < numBooks; i++) {
        books[i].position = (Vector2){ GetRandomValue(100, SCREEN_WIDTH - 100), GetRandomValue(100, SCREEN_HEIGHT - 100) };
        books[i].texture = LoadTexture("C:/Users/Casa/Music/Pictures/cBook_sprite.png");
        books[i].collected = false;
    }*/

    // Set player position
    player.position = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

    // Set zombie position inside the walls
    int randX, randY;
    do {
        randX = GetRandomValue(walls[0].rect.x + 10, walls[3].rect.x - 10);
        randY = GetRandomValue(walls[1].rect.y + 10, walls[2].rect.y - 10);
    } while (CheckCollisionCircleRec((Vector2){ randX, randY }, zombie.radius, walls[0].rect) ||
             CheckCollisionCircleRec((Vector2){ randX, randY }, zombie.radius, walls[1].rect) ||
             CheckCollisionCircleRec((Vector2){ randX, randY }, zombie.radius, walls[2].rect) ||
             CheckCollisionCircleRec((Vector2){ randX, randY }, zombie.radius, walls[3].rect));

    zombie.position = (Vector2){ randX, randY };
    
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


    // Check for collisions with walls
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionCircleRec(player.position, player.radius, walls[i].rect) ||
            CheckCollisionCircleRec(zombie.position, zombie.radius, walls[i].rect)) {
            // Check for collisions with walls for player
    if (player.position.x - player.radius < walls[0].rect.x) player.position.x = walls[0].rect.x + player.radius;
    if (player.position.x + player.radius > walls[3].rect.x + walls[3].rect.width) player.position.x = walls[3].rect.x + walls[3].rect.width - player.radius;
    if (player.position.y - player.radius < walls[1].rect.y) player.position.y = walls[1].rect.y + player.radius;
    if (player.position.y + player.radius > walls[2].rect.y + walls[2].rect.height) player.position.y = walls[2].rect.y + walls[2].rect.height - player.radius;

    // Check for collisions with walls for zombie
    if (zombie.position.x - zombie.radius < walls[0].rect.x) zombie.position.x = walls[0].rect.x + zombie.radius;
    if (zombie.position.x + zombie.radius > walls[3].rect.x + walls[3].rect.width) zombie.position.x = walls[3].rect.x + walls[3].rect.width - zombie.radius;
    if (zombie.position.y - zombie.radius < walls[1].rect.y) zombie.position.y = walls[1].rect.y + zombie.radius;
    if (zombie.position.y + zombie.radius > walls[2].rect.y + walls[2].rect.height) zombie.position.y = walls[2].rect.y + walls[2].rect.height - zombie.radius;

        }
    }
    
    // Update player movement
    Vector2 playerDirection  = { 0 }; // Initialize a direction vector with (0, 0)

    if (IsKeyDown(KEY_UP)) playerDirection .y -= 1; // Up
    if (IsKeyDown(KEY_DOWN)) playerDirection .y += 1; // Down
    if (IsKeyDown(KEY_LEFT)) playerDirection .x -= 1; // Left
    if (IsKeyDown(KEY_RIGHT)) playerDirection .x += 1; // Right

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

    // Draw grass texture outside the walls
    for (int y = 0; y < SCREEN_HEIGHT; y += grassTexture.height) {
        for (int x = 0; x < SCREEN_WIDTH; x += grassTexture.width) {
            DrawTexture(grassTexture, x, y, WHITE);
        }
    }

    // Draw walls
    for (int i = 0; i < 12; i++) {
        DrawRectangleRec(walls[i].rect, GRAY);
    }
    
    // Draw the number of books collected
    char booksCollectedText[30];
    snprintf(booksCollectedText, 20, "Livros Coletados: %d", booksCollected);
    DrawText(booksCollectedText, 20, 20, 20, BLACK);

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
    
    InitAudioDevice();
    
    // Load background music file (e.g., "C:/Users/Casa/Music/song.mp3")
    bgMusic = LoadMusicStream("C:/Users/Casa/Music/BossTheme.wav");

    // Set music volume (optional)
    SetMusicVolume(bgMusic, 0.8f);

    // Start playing the background music
    PlayMusicStream(bgMusic);
    
    AskPlayerName();

    // Initialize the current game state as STATE_MENU
    GameState currentGameState = STATE_MENU;

    while (!WindowShouldClose()) {
        // Update the music stream (required to keep the music playing)
        UpdateMusicStream(bgMusic);
        // Handle state transitions based on user input
        switch(currentGameState) {
            case STATE_MENU:
                DrawMenu();
                if (IsKeyPressed(KEY_ONE)) {
                    currentGameState = STATE_PLAY;
                    InitGame();
                } else if (IsKeyPressed(KEY_TWO)) {
                    currentGameState = STATE_RANKING;
                } else if (IsKeyPressed(KEY_THREE)) {
                    currentGameState = STATE_CREDITS;
                } else if (IsKeyPressed(KEY_FOUR)) {
                    currentGameState = STATE_DONATIONS;
                }
                break;
            case STATE_PLAY:
                UpdateGame();
                DrawGame();
                break;
            case STATE_RANKING:      
                LoadScores(); // Load scores from the file
                DrawScores();
                
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentGameState = STATE_MENU;
                }
                break;
            case STATE_CREDITS:
                DrawCredits(); // Placeholder function
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentGameState = STATE_MENU;
                }
                break;
            case STATE_DONATIONS:
                DrawDonations(); // Placeholder function
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentGameState = STATE_MENU;
                }
                break;
            default:
                break;
        }
    }

    // Unload textures when closing the window
    UnloadTexture(player.texture);
    UnloadTexture(zombie.texture);
    UnloadTexture(grassTexture);
    UnloadMusicStream(bgMusic);
    for (int i = 0; i < numBooks; i++) {
        if (!books[i].collected)
            UnloadTexture(books[i].texture);
    }
    
    CloseAudioDevice(); // Close the audio device when done
    CloseWindow();

    return 0;
}

void DrawMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw menu options
    DrawText("1 - Jogar", SCREEN_WIDTH / 2 - MeasureText("1 - Jogar", 30) / 2, SCREEN_HEIGHT / 2 - 80, 30, DARKGRAY);
    DrawText("2 - Ranking", SCREEN_WIDTH / 2 - MeasureText("2 - Ranking", 30) / 2, SCREEN_HEIGHT / 2 - 40, 30, DARKGRAY);
    DrawText("3 - Créditos", SCREEN_WIDTH / 2 - MeasureText("3 - Créditos", 30) / 2, SCREEN_HEIGHT / 2, 30, DARKGRAY);
    DrawText("4 - Doações", SCREEN_WIDTH / 2 - MeasureText("4 - Doações", 30) / 2, SCREEN_HEIGHT / 2 + 40, 30, DARKGRAY);

    EndDrawing();
}

void DrawCredits(){
    BeginDrawing();
    
    // Clear the background
    ClearBackground(RAYWHITE);

    // Draw credits text
    DrawText("Créditos:", 50, 50, 40, BLACK);
    DrawText("- Lázaro Felipe (Programador)", 100, 150, 20, DARKGRAY);
    DrawText("- Luan Nathan (Programador)", 100, 200, 20, DARKGRAY);
    DrawText("- Gabriel Vieira (Programador)", 100, 250, 20, DARKGRAY);
    
    EndDrawing();
}
void DrawDonations(){
    BeginDrawing();
    
    // Clear the background
    ClearBackground(RAYWHITE);

    // Draw donations text
    DrawText("Doações:", 50, 50, 40, BLACK);
    DrawText("Nos apóie em: UrubuDoPix@gmail.com", 100, 150, 20, DARKGRAY);
    
    EndDrawing();
}

void AskPlayerName() {
    bool nameEntered = false;
    char playerName[20] = ""; // Variable to store the player's name

    while (!nameEntered) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Digite seu nome:", 10, 10, 30, BLACK);
        DrawText(playerName, 10, 50, 30, BLACK); // Draw the player's name

        if (IsKeyPressed(KEY_ENTER) && strlen(playerName) > 0) {
            SaveScore(playerName, booksCollected); // Save the player's score with their name
            nameEntered = true;
        } else {
            // Get the player's name input
            int key = GetKeyPressed();
            if ((key >= 32) && (key <= 125) && (strlen(playerName) < 19)) {
                char letter = (char)key;
                int length = strlen(playerName);
                playerName[length] = letter;
                playerName[length + 1] = '\0';
            }
        }

        // Erase last character with backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int length = strlen(playerName);
            if (length > 0) {
                playerName[length - 1] = '\0';
            }
        }

        EndDrawing();
    }
}


// Function to save the player's score to a file
void SaveScore(const char* playerName, int score) {
    FILE* file = fopen("scores.txt", "a"); // Open the file in append mode

    if (file) {
        fprintf(file, "%s,%d\n", playerName, score); // Write player's name and score to the file
        fclose(file); // Close the file
    }
}

// Function to load scores from the file
void LoadScores() {
    FILE* file = fopen("scores.txt", "r"); // Open the file in read mode

    if (file) {
        // Read scores from the file and store them in the scores array
        int index = 0;
        while (fscanf(file, "%19[^,],%d\n", scores[index].playerName, &scores[index].score) == 2) {
            index++;
            if (index >= MAX_SCORES) {
                break;
            }
        }
        fclose(file); // Close the file
    }
}

// Function to draw the scores on the ranking menu
void DrawScores() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Sort the scores in descending order
    for (int i = 0; i < MAX_SCORES - 1; i++) {
        for (int j = i + 1; j < MAX_SCORES; j++) {
            if (scores[i].score < scores[j].score) {
                ScoreEntry temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Draw the top 10 scores
    DrawText("Ranking:", 50, 50, 40, BLACK);
    for (int i = 0; i < 10; i++) {
        DrawText(TextFormat("%d. %s - %d", i + 1, scores[i].playerName, scores[i].score), 100, 150 + i * 30, 20, DARKGRAY);
    }

    EndDrawing();
}
