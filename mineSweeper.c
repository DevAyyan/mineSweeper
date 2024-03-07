#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define MOVESIZE 526 // (25 * 25 - 99)

int SIDE; // side length of the board
int MINES; // number of mines on the board


void titlescreen();
bool isValid(int, int);
bool isMine(int, int , char [][SIDE]);
void makeMove(int *, int *);
void printBoard(char [][SIDE]);
int countAdjacentMines(int , int , int [][2], char [][SIDE]);
bool playMinesweeperUtil(char [][SIDE], char [][SIDE], int [][2], int , int , int *);
void placeMines(int [][2], char [][SIDE]);
void initialise(char [][SIDE], char [][SIDE]);
void replaceMine(int , int , char [][SIDE]);
void playMinesweeper();
void chooseDifficultyLevel();

int main() {
	titlescreen();
    chooseDifficultyLevel();
    playMinesweeper();
    
    return 0;
}

// the titlescreen
void titlescreen(){
	    printf("\n");
    printf("    __  __              _____\n");
    printf("   /  |/  ()____  ___  / ___/     _____  ___  ____  ___  _____\n");
    printf("  / /|/  / / __ \\/ _ \\ \\_ \\ | /| / / _ \\/ _ \\/ __ \\/ _ \\/ ___/\n");
    printf(" / /  / / / / / /  __/__/ / |/ |/ /  __/  __/ /_/ /  __/ /\n");
    printf("/_/  /_/_/_/ /_/\\___/____/|__/|__/\\___/\\___/ .___/\\___/_/\n");
    printf("                                          /_/\n");
    printf(" A Project created by \n Muhammad Ayyan Amir (23k-3010)\n Muhammad Wadeed Khan (23k-3026)\n Abdul Kabeer Imtiaz (23k-3004)\n\n");
    printf(" Press any key to start...");
    
    getch();
    system("cls");
}

// A Utility Function to check whether given cell (row, col) is a valid cell or not
bool isValid(int row, int col) {
    return (row >= 0) && (row < SIDE) && (col >= 0) && (col < SIDE);
}

// A Utility Function to check whether given cell (row, col) has a mine or not.
bool isMine(int row, int col, char board[][SIDE]) {
    return (board[row][col] == '*');
}

// A Function to get the user's move
void makeMove(int *x, int *y) {
    printf(" Enter your move, (row, column) -> ");
    scanf("%d %d", x, y);
    *x = *x - 1;
    *y = *y - 1;
    system("cls");
}

// A Function to print the current gameplay board
void printBoard(char myBoard[][SIDE]) {
    int i, j;

    printf("   ");

    for (i = 1; i <= SIDE; i++)
        printf(" %2d ", i);

    printf("\n");

    for (i = 1; i <= SIDE; i++) {
        printf("%2d ", i);

        for (j = 1; j <= SIDE; j++) {
            printf("  %c ", myBoard[i - 1][j - 1]);
        }

        printf(" \n");

        if (i < SIDE) {
            printf("   ");
            for (j = 1; j <= SIDE; j++)
                printf("    ");
            printf("\n");
        }
    }
}



// A Function to count the number of mines in the adjacent cells
int countAdjacentMines(int row, int col, int mines[][2], char realBoard[][SIDE]) {
    int i;
    int count = 0;

    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (i = 0; i < 8; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];

        if (isValid(newRow, newCol) && isMine(newRow, newCol, realBoard))
            count++;
    }

    return count;
}

// A Recursive Function to play the Minesweeper Game
bool playMinesweeperUtil(char myBoard[][SIDE], char realBoard[][SIDE], int mines[][2], int row, int col, int *movesLeft) {
    if (myBoard[row][col] != '-')
        return false;

    int i, j;

    if (realBoard[row][col] == '*') {
        myBoard[row][col] = '*';

        for (i = 0; i < MINES; i++)
            myBoard[mines[i][0]][mines[i][1]] = '*';

        printBoard(myBoard);
        printf("\n You lost!\n");
        printf("\nPress any key to exit...");
        getch(); 

        return true;
    } else {
        int count = countAdjacentMines(row, col, mines, realBoard);
        (*movesLeft)--;

        myBoard[row][col] = (count == 0) ? ' ' : (count + '0');

        if (count == 0) {
            for (i = row - 1; i <= row + 1; i++) {
                for (j = col - 1; j <= col + 1; j++) {
                    if (isValid(i, j) && myBoard[i][j] == '-')
                        playMinesweeperUtil(myBoard, realBoard, mines, i, j, movesLeft);
                }
            }
        }

        return false;
    }
}

// A Function to place the mines randomly on the board
void placeMines(int mines[][2], char realBoard[][SIDE]) {
    bool mark[SIDE * SIDE];
    memset(mark, false, sizeof(mark));

    srand(time(NULL));

    for (int i = 0; i < MINES;) {
        int random = rand() % (SIDE * SIDE);
        int x = random / SIDE;
        int y = random % SIDE;

        if (!mark[random]) {
            mines[i][0] = x;
            mines[i][1] = y;

            realBoard[x][y] = '*';
            mark[random] = true;
            i++;
        }
    }
}

// A Function to initialise the game
void initialise(char realBoard[][SIDE], char myBoard[][SIDE]) {

    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            myBoard[i][j] = realBoard[i][j] = '-';
        }
    }
}

// A function to replace the mine from (row, col) and put it to a vacant space
void replaceMine(int row, int col, char board[][SIDE]) {
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (board[i][j] != '*') {
                board[i][j] = '*';
                board[row][col] = '-';
                return;
            }
        }
    }
}

// A Function to play Minesweeper game
void playMinesweeper() {
    bool gameOver = false;
    char realBoard[SIDE][SIDE], myBoard[SIDE][SIDE];
    int movesLeft = SIDE * SIDE - MINES, x, y;
    int mines[MINES][2]; // stores (x,y) coordinates of all mines.

    initialise(realBoard, myBoard);
    placeMines(mines, realBoard);

    int currentMoveIndex = 0;
    while (!gameOver) {
        printf(" Current Status of Board :\n");
        printBoard(myBoard);
        makeMove(&x, &y);

        if (currentMoveIndex == 0 && isMine(x, y, realBoard)) {
            replaceMine(x, y, realBoard);
        }

        currentMoveIndex++;

        gameOver = playMinesweeperUtil(myBoard, realBoard, mines, x, y, &movesLeft);

        if (!gameOver && movesLeft == 0) {
        	printBoard(myBoard);
            printf("\n You won!\n");
            gameOver = true;
            printf("\nPress any key to exit...");
            getch(); 
        }
    }
}

// A Function to choose the difficulty level of the game
void chooseDifficultyLevel() {
    int level;

    do{
		printf("\n Choose your difficulty:\n 1. Beginner.\n 2. Intermediate.\n 3. Advanced.\n");
		scanf("%d", &level);

		switch(level){
			case 1: SIDE = 9;
					MINES = 10;
					break;
			case 2: SIDE = 16;
					MINES = 40;
					break;
			case 3: SIDE = 24;
					MINES = 99;
					break;
			default: 
			printf(" Wrong number entered.");
			break;
		}
	} while(SIDE == 0 || MINES == 0);
	
	system("cls");
}