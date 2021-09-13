// LabyGame.cpp

#include <iostream>
#include <conio.h>


#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iterator>     // std::distance

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define END 27
#define SPACE 32

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

const int WIDTH = 13;
const int HEIGHT = 43;

ofstream temp("output.txt", ios::trunc);
ofstream dada("output.txt", ios::app);

//const int WIDTH = 53;
//const int HEIGHT = 231;

const int buffer_size = WIDTH * HEIGHT * 8;
bool instaPrint = true;


COORD coord;



void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    cout << "\x1B[2J\x1B[H";
}








class Game {
public:
    int currentPos[2] = { 1, 1 };
    char maze[WIDTH][HEIGHT];
    bool running = false;


    void generateMaze() {
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                maze[x][y] = '#';
            }
        }
        currentPos[0] = 1;
        currentPos[1] = 1;

        Visit(1, 1);

        clear();

        maze[1][1] = 'O';
        maze[WIDTH-2][HEIGHT-1] = ' ';
        if (instaPrint) {
            print();
        }

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                dada << maze[x][y];
            }
            dada << endl;
        }

    }

    void Visit(int x, int y)
    {
        // Starting at the given index, recursively visits every direction in a
        // randomized order.
        // Set my current location to be an empty passage.
        maze[x][y] = ' ';
        // Create an local array containing the 4 directions and shuffle their order.
        int dirs[4];
        dirs[0] = NORTH;
        dirs[1] = EAST;
        dirs[2] = SOUTH;
        dirs[3] = WEST;

        for (int i = 0; i < 4; ++i)
        {
            int r = rand() & 3;
            int temp = dirs[r];
            dirs[r] = dirs[i];
            dirs[i] = temp;
        }

        // Loop through every direction and attempt to Visit that direction.
        for (int i = 0; i < 4; ++i)
        {
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
            if (IsInBounds(x2, y2))
            {
                if (maze[x2][y2] == '#')
                {
                    // (x2,y2) has not been visited yet... knock down the
                    // wall between my current position and that position
                    maze[x2 - dx][ y2 - dy] = ' ';
                    // Recursively Visit (x2,y2)

                    //clear();
                    if (!instaPrint) {
                        print();
                    }

                    Visit(x2, y2);


                }
            }

            
        }
    }



    void print() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        static char buffer[buffer_size];
        char* p_next_write = &buffer[0];
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                *p_next_write++ = maze[x][y];
            }
            *p_next_write++ = '\n';
        }
        *p_next_write = '\0';
        cout.write(&buffer[0], buffer_size);
    }


    void run() {
        running = true;
        while (running) {
            bool work = false;
            switch (_getch()) {
                case KEY_UP:
                    if (moveUp())
                        work = true;
                    //generateMaze();
                    break;
                case KEY_DOWN:
                    if (moveDown())
                        work = true;
                    //generateMaze();
                    break;
                case KEY_LEFT:
                    if (moveLeft())
                        work = true;
                    //generateMaze();
                    break;
                case KEY_RIGHT:
                    if (moveRight())
                        work = true;
                    //generateMaze();
                    break;
                case END:
                    running = false;
                    break;
                default:
                    break;
            }
            if (work) {
                //clear();
                print();
                if (checkWin(currentPos[0], currentPos[1])) {
                    clear();
                    running = false;
                    cout << "You won!" << endl;
                }
            }
        }
    }

    bool moveUp() {
        if (!checkObstacle(currentPos[0]-1, currentPos[1]) &&
            IsInBounds(currentPos[0] - 1, currentPos[1]) ) {
            maze[currentPos[0]][currentPos[1]] = ' ';
            maze[currentPos[0]-1][currentPos[1]] = 'O';
            currentPos[0] = currentPos[0] - 1;
            return true;
        }
        return false;
    }

    bool moveDown() {
        if (!checkObstacle(currentPos[0] + 1, currentPos[1]) &&
            IsInBounds(currentPos[0] + 1, currentPos[1])) {
            maze[currentPos[0]][currentPos[1]] = ' ';
            maze[currentPos[0] + 1][currentPos[1]] = 'O';
            currentPos[0] = currentPos[0] + 1;
            return true;
        }
        return false;
    }

    bool moveRight() {
        if (!checkObstacle(currentPos[0], currentPos[1] + 1) &&
            IsInBounds(currentPos[0], currentPos[1] + 1) ) {
            maze[currentPos[0]][currentPos[1]] = ' ';
            maze[currentPos[0]][currentPos[1]+1] = 'O';
            currentPos[1] = currentPos[1] + 1;
            return true;
        }
        return false;
        
    }

    bool moveLeft() {
        if (!checkObstacle(currentPos[0], currentPos[1] - 1) &&
            IsInBounds(currentPos[0], currentPos[1] - 1)) {
            maze[currentPos[0]][currentPos[1]] = ' ';
            maze[currentPos[0]][currentPos[1] - 1] = 'O';
            currentPos[1] = currentPos[1] - 1;
            return true;
        }
        return false;
        
    }

    bool checkWin(int x, int y) {
        if (x == WIDTH - 2 && y == HEIGHT - 1) {
            return true;
        }
        return false;
    }


    bool checkObstacle(int x, int y) {
        if (maze[x][y] == '#') {
            return true;
        }
        return false;
    }






    bool IsInBounds(int x, int y) {
        if (x < 0 || x >= WIDTH) return false;
        if (y < 0 || y >= HEIGHT) return false;
        return true;

    }


};




int main()
{
    temp.close();
    
    srand(time(NULL));
    bool running = true;
    Game game;

    coord.X = 0;
    coord.Y = 0;

    cout << "Press SPACE to start new game..." << endl;
    cout << "Press ESC to cancel" << endl;
    cout << "Controlls: ARROW KEYS, ESC to end." << endl;

    while (running) {
        
        


        switch (_getch()) {
        case SPACE:
            game.generateMaze();
            game.print();
            game.run();

            cout << "Press SPACE to start new game..." << endl;
            cout << "Press ESC to cancel" << endl;
            cout << "Controlls: ARROW KEYS, ESC to end." << endl;


            break;
        case END:
            running = false;
            break;
        case KEY_UP:
            //clear();
            game.generateMaze();
            game.print();
            break;
        default:
            clear();
            break;
        }
        
    }

    dada.close();


}








