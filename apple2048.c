#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"


void printInstructions(void)
{
    int seed = 0;

    clrscr();
    printf(
// 0000000001111111111222222222233333333334
// 1234567890123456789012345678901234567890
  "              APPLE 2048\n"
  "\n"
  "USE I-J-K-M OR THE ARROW KEYS TO SLIDE\n"
  "ALL TILES IN A DIRECTION.  MATCHING\n"
  "TILES ARE ADDED TOGETHER TO MAKE A NEW\n"
  "TILE.  ON EVERY MOVE, ONE MORE TILE IS\n"
  "ADDED WITH A RANDOM VALUE OF EITHER 2\n"
  "OR 4.\n"
  "\n"
  "PLAY ENDS WHEN ALL TILES ARE OCCUPIED\n"
  "AND NO MORE MOVES ARE POSSIBLE.  TRY\n"
  "TO GET THE LARGEST TILE YOU CAN!\n"
  "\n"
  "PRESS ESCAPE OR Q TO QUIT AT ANY TIME.\n"
  "PRESS R TO START A NEW GAME.\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "          PRESS ANY KEY TO START");

    while (!kbhit())
        seed++;

    cgetc();
    srand(seed);
}


void printBoard(void)
{
    tPos x;
    tPos y;

    clrscr();

    for (y = 1; y <= BOARD_SIZE; y++) {
        printf("+");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("--------+");
        }
        printf("\n!");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("        !");
        }
        printf("\n!");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("%s!", tileStringForPos(x, y));
        }
        printf("\n!");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("        !");
        }
        printf("\n");
    }
    printf("+");
    for (x = 1; x <= BOARD_SIZE; x++) {
        printf("--------+");
    }
    printf("\n\nCURRENT SCORE: %ld\nTRY TO GET THE %ld TILE!", currentScore(),
            nextTarget());
}


void gameWon(void)
{
    printf("\nCONGRATULATIONS, YOU HAVE WON THE GAME!\n");
    cgetc();
    exit(0);
}


void gameLost(void)
{
    printf("\nSORRY, NO MORE MOVES.\n");
    cgetc();
    exit(0);
}


void handleNextEvent(void)
{
    char ch;

    while (true) {
        ch = cgetc();
        switch (ch) {
            case 'i':
            case 'I':
            case 11:        // Up arrow
                slideInDirection(DIR_UP);
                return;

            case 'j':
            case 'J':
            case 8:         // Left arrow
                slideInDirection(DIR_LEFT);
                return;

            case 'k':
            case 'K':
            case 21:        // Right arrow
                slideInDirection(DIR_RIGHT);
                return;

            case 'm':
            case 'M':
            case 10:        // Up arrow
                slideInDirection(DIR_DOWN);
                return;

            case 27:        // Escape
            case 'q':
            case 'Q':
                exit(0);

            case 'r':
            case 'R':
                initGame();
                return;
        }
    }
}


int main(void)
{
    printInstructions();

    initGame();

    while (true) {
        printBoard();

        if (isGameWon())
            gameWon();

        if (isGameLost())
            gameLost();

        handleNextEvent();
    }

    return 0;
}
