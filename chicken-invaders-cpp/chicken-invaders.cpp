#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime> //for random time generate
using namespace std;

// Constants
const int width = 50;
const int height = 20;
const int totalbullets = 20;
const int totalchicken = 5;
const int totaleggs = 10;
const char spaceship_logo = '$';
const char chicken_logo = '&';
const char egg_logo = 'o';
const char bullet_logo = '*';
const char empty_logo = ' ';

// Game Variables
int spaceshipX = width / 2;
int spaceshipY = height - 2;
int lives = 3;
int score = 0;

int bulletX[totalbullets];
int bulletY[totalbullets];
bool working_bullet[totalbullets];

int chickenX[totalchicken];
int chickenY[totalchicken];
bool working_chicken[totalchicken];

int eggX[totaleggs];
int eggY[totaleggs];
bool working_eggs[totaleggs];

bool isMovingRight = true;

// Function Prototypes
void startgame();
void clearScreen(char screen[][width + 2]);
void renderScreen(char screen[][width + 2]);
void drawGameElements(char screen[][width + 2]);
void handleInput();
void updateBullets();
void updateChickens();
void dropEggs();
void updateEggs();
void checkCollisions();
void shootBullet();
void delay();
void setCursorPosition(int x, int y);
void menu();
// Main Function
int main() {

    srand(static_cast<unsigned int>(time(0)));  // Seed random number generator
    char screen[height + 2][width + 2];  // Buffer for the screen

    startgame();
    for (int i = 1; i <= 5; i++)
    {
        cout << endl;
    }
    cout << "\t\t\t\t|--------------------------------------------|" << endl;
    cout << "\t\t\t\t|                    Menu                    |" << endl;
    cout << "\t\t\t\t|--------------------------------------------|" << endl;
    cout << "\t\t\t\t|| 1. Start                                 ||" << endl;
    cout << "\t\t\t\t|| 2. High Score                            ||" << endl;
    cout << "\t\t\t\t|| 3. Exit                                  ||" << endl;
    cout << "\t\t\t\t|--------------------------------------------|" << endl;
    int choice;
    cin >> choice;
    system("cls");
    while (lives > 0) {
        clearScreen(screen);  // Reset the screen buffer
        handleInput();        // Handle user input
        updateBullets();      // Update bullet positions
        updateChickens();     // Update chicken positions
        dropEggs();           // Drop eggs randomly
        updateEggs();         // Move eggs downward
        checkCollisions();    // Handle collisions
        drawGameElements(screen); // Populate screen buffer
        renderScreen(screen);  // Render the screen
        delay();               // Control frame rate
    }

    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}

// Initialize the game state
void startgame() {
    fill(begin(working_bullet), end(working_bullet), false);
    for (int i = 0; i < totalchicken; i++) {
        chickenX[i] = rand() % width;
        chickenY[i] = rand() % (height / 2);
        working_chicken[i] = true;
    }
    fill(begin(working_eggs), end(working_eggs), false);
}

// Clear the screen buffer
void clearScreen(char screen[][width + 2]) {
    for (int y = 0; y < height + 2; y++) {
        for (int x = 0; x < width + 2; x++) {
            screen[y][x] = (y == 0 || y == height + 1 || x == 0 || x == width + 1) ? '*' : empty_logo;
        }
    }
}

// Render the screen
void renderScreen(char screen[][width + 2]) {
    setCursorPosition(0, 0);  // Move cursor to top-left corner
    for (int y = 0; y < height + 2; y++) {
        for (int x = 0; x < width + 2; x++) {
            cout << screen[y][x];
        }
        cout << endl;
    }
    cout << "Score: " << score << " | Lives: " << lives << endl;
}

// Draw game elements on the screen buffer
void drawGameElements(char screen[][width + 2]) {
    screen[spaceshipY + 1][spaceshipX + 1] = spaceship_logo;  // Draw spaceship
    for (int i = 0; i < totalbullets; i++) {
        if (working_bullet[i]) {
            screen[bulletY[i] + 1][bulletX[i] + 1] = bullet_logo;
        }
    }
    for (int i = 0; i < totalchicken; i++) {
        if (working_chicken[i]) {
            screen[chickenY[i] + 1][chickenX[i] + 1] = chicken_logo;
        }
    }
    for (int i = 0; i < totaleggs; i++) {
        if (working_eggs[i]) {
            screen[eggY[i] + 1][eggX[i] + 1] = egg_logo;
        }
    }
}

// Handle player input
void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'w' && spaceshipY > 0) spaceshipY--;
        else if (key == 's' && spaceshipY < height - 1) spaceshipY++;
        else if (key == 'a' && spaceshipX > 0) spaceshipX--;
        else if (key == 'd' && spaceshipX < width - 1) spaceshipX++;
        else if (key == ' ') shootBullet();
    }
}

// Shoot a bullet
void shootBullet() {
    for (int i = 0; i < totalbullets; i++) {
        if (!working_bullet[i]) {
            bulletX[i] = spaceshipX;
            bulletY[i] = spaceshipY - 1;
            working_bullet[i] = true;
            break;
        }
    }
}

// Move bullets upward
void updateBullets() {
    for (int i = 0; i < totalbullets; i++) {
        if (working_bullet[i]) {
            bulletY[i]--;
            if (bulletY[i] < 0) working_bullet[i] = false;
        }
    }
}

// Move chickens
void updateChickens() {
    for (int i = 0; i < totalchicken; i++) {
        if (working_chicken[i]) {
            chickenX[i] += isMovingRight ? 1 : -1;
            if (chickenX[i] == 0 || chickenX[i] == width - 1) {
                isMovingRight = !isMovingRight;
                chickenY[i]++;
            }
        }
    }
}

// Drop eggs randomly
void dropEggs() {
    for (int i = 0; i < totalchicken; i++) {
        if (working_chicken[i] && rand() % 10 == 0) {
            for (int j = 0; j < totaleggs; j++) {
                if (!working_eggs[j]) {
                    eggX[j] = chickenX[i];
                    eggY[j] = chickenY[i] + 1;
                    working_eggs[j] = true;
                    break;
                }
            }
        }
    }
}

// Move eggs downward
void updateEggs() {
    for (int i = 0; i < totaleggs; i++) {
        if (working_eggs[i]) {
            eggY[i]++;
            if (eggY[i] >= height) working_eggs[i] = false;
        }
    }
}

// Check collisions
void checkCollisions() {
    for (int i = 0; i < totalbullets; i++) {
        if (working_bullet[i]) {
            for (int j = 0; j < totalchicken; j++) {
                if (working_chicken[j] && bulletX[i] == chickenX[j] && bulletY[i] == chickenY[j]) {
                    working_bullet[i] = false;
                    working_chicken[j] = false;
                    score += 10;
                }
            }
        }
    }
    for (int i = 0; i < totaleggs; i++) {
        if (working_eggs[i] && eggX[i] == spaceshipX && eggY[i] == spaceshipY) {
            working_eggs[i] = false;
            lives--;
        }
    }
}

// Delay
void delay() {
    Sleep(70);
}

// Set cursor position
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
