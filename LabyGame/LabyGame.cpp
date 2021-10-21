#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <iterator> 
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define W 'w'
#define S 's'
#define A 'a'
#define D 'd'

#define END 27
#define SPACE 32

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define WALL -37
#define WALL_DAMAGED 1
#define WALL_BROKEN 2


#pragma region Definicije

// menus
void mainMenu();
void modeSelector();
void statsMenu();

// settings
void loadSettings();
void saveSettings();

// game
void playerInput();
void run();
void generateMap(int x, int y); // rekruzivna funkcija za generacijo mape
void generatePowerUps();
void generatePickups();

bool moveUp(int player);
bool moveDown(int player);
bool moveLeft(int player);
bool moveRight(int player);



// collission logic
bool isInBounds(int x, int y);
bool isObstacle(int x, int y);
bool isPowerUp(int x, int y);
bool isPoint(int x, int y);


// effects
void explosion(int player);
void rocket(int player, int direction);

// Graphics
void render();


#pragma endregion Definicije


#pragma region Globalne Spremenljivke
const int WIDTH = 13;
const int HEIGHT = 43;
/// FULL SCREEN
//const int WIDTH = 61;
//const int HEIGHT = 237;


const int buffer_size = WIDTH * HEIGHT * 8;

COORD coord;

char map[HEIGHT][WIDTH] = {};
int playerPositions[2][2] = { {0,0}, {0,0} };
int players[2] = {'X', 'Y'};
int powerUps[20] = {};
int pickups[20] = {};

bool running = false;


#pragma endregion Globalne Spremenljivke

#pragma region Nastavitve
ofstream scores("scores.txt", ios::app);
fstream settingsFile("settings.txt", ios::in);

bool instaPrint = false; // prikaz generiranja labirinta

#pragma endregion Nastavitve








bool IsInBounds(int x, int y) {
    if (x < 0 || x >= WIDTH) return false;
    if (y < 0 || y >= HEIGHT) return false;
    return true;

}


void print() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    static char buffer[buffer_size];
    char* p_next_write = &buffer[0];
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            *p_next_write++ = map[x][y];
        }
        *p_next_write++ = '\n';
    }
    *p_next_write = '\0';
    cout.write(&buffer[0], buffer_size);
}




bool moveUp(int player) {

    if (!isObstacle(playerPositions[player][0], playerPositions[player][1] - 1) &&
        IsInBounds(playerPositions[player][1], playerPositions[player][1] - 1)) {
        playerPositions[player][1] = playerPositions[player][1] - 1;
        return true;
    }
    return false;
}

bool moveDown(int player) {
    if (!isObstacle(playerPositions[player][0], playerPositions[player][1] + 1) &&
        IsInBounds(playerPositions[player][0], playerPositions[player][1] + 1)) {
        playerPositions[player][1] = playerPositions[player][1] + 1;
        return true;
    }
    return false;
}

bool moveRight(int player) {
    if (!isObstacle(playerPositions[player][0] + 1, playerPositions[player][1]) &&
        IsInBounds(playerPositions[player][0] + 1, playerPositions[player][1])) {
        playerPositions[player][0] = playerPositions[player][0] + 1;
        return true;
    }
    return false;

}

bool moveLeft(int player) {
    if (!isObstacle(playerPositions[player][0] - 1, playerPositions[player][1]) &&
        IsInBounds(playerPositions[player][0] - 1, playerPositions[player][1])) {
        playerPositions[player][0] = playerPositions[player][0] - 1;
        return true;
    }
    return false;

}






void generateMap(int x, int y){
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    map[x][y] = ' ';
    // Create an local array containing the 4 directions and shuffle their order.
    int dirs[4];
    dirs[0] = NORTH;
    dirs[1] = EAST;
    dirs[2] = SOUTH;
    dirs[3] = WEST;

    for (int i = 0; i < 4; ++i){
        int r = rand() & 3;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }

    // Loop through every direction and attempt to Visit that direction.
    for (int i = 0; i < 4; ++i){
        // dx,dy are offsets from current location. Set them based
        // on the next direction I wish to try.
        int dx = 0, dy = 0;
        switch (dirs[i])
        {
        case NORTH: dy = -1; break;
        case SOUTH: dy = 1; break;
        case EAST: dx = 1; break;
        case WEST: dx = -1; break;
        }
        // Find the (x,y) coordinates of the grid cell 2 spots
        // away in the given direction.
        int x2 = x + (dx << 1);
        int y2 = y + (dy << 1);
        if (IsInBounds(x2, y2)){
            //cout <<(int) maze[x2][y2];
            if (map[x2][y2] == WALL){
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                map[x2 - dx][y2 - dy] = ' ';
                // Recursively Visit (x2,y2)
                if (!instaPrint) {
                    render();
                }
                generateMap(x2, y2);


            }
        }


    }
}


void playerInput() {
    for (;;) {
        if (_kbhit) {
            switch (_getch()) {
            case KEY_UP:
                moveUp(0);
                break;
            case KEY_DOWN:
                moveDown(0);
                break;
            case KEY_LEFT:
                moveLeft(0);
                break;
            case KEY_RIGHT:
                moveRight(0);
                break;
            case W:
                moveUp(1);
                break;
            case S:
                moveDown(1);
                break;
            case A:
                moveLeft(1);
                break;
            case D:
                moveRight(1);
                break;
            default:
                break;
            }
        }
    }
}





int main(){
    mainMenu();


}
