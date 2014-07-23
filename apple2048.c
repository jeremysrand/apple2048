/*
 * File: apple2048.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the entry point for the 2048 game.
 */

#include <apple2enh.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"


#define TILE_WIDTH 10
#define TILE_HEIGHT 5

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

    // The amount of time the user waits to read the in
    while (!kbhit())
        seed++;

    cgetc();
    srand(seed);

    clrscr();
}


void printBoard(void)
{
    tPos x;
    tPos y;

    for (x = 1; x <= BOARD_SIZE; x++) {
        for (y = 1; y <= BOARD_SIZE; y++) {
            textframexy((x - 1) * TILE_WIDTH,
                        (y - 1) * TILE_HEIGHT,
                        TILE_WIDTH,
                        TILE_HEIGHT,
                        TEXTFRAME_WIDE);
            cputsxy((x - 1) * TILE_WIDTH + 1,
                    (y - 1) * TILE_HEIGHT + 2,
                    tileStringForPos(x, y));
        }
    }

    gotoxy(0,20);
    printf("CURRENT SCORE: %ld\nTRY TO GET THE %ld TILE!", currentScore(),
            nextTarget());
}


void gameWon(void)
{
    gotoxy(0, 22);
    printf("YOU HAVE WON THE GAME!  PRESS ANY KEY...\n");
    cgetc();
    initGame();
}


void gameLost(void)
{
    gotoxy(0, 22);
    printf("SORRY, NO MORE MOVES.  PRESS ANY KEY...");
    cgetc();
    initGame();
}


void handleNextEvent(void)
{
    char ch;

    while (true) {
        ch = cgetc();
        switch (ch) {
            case 'i':
            case 'I':
            case CH_CURS_UP:
                slideInDirection(DIR_UP);
                return;

            case 'j':
            case 'J':
            case CH_CURS_LEFT:
                slideInDirection(DIR_LEFT);
                return;

            case 'k':
            case 'K':
            case CH_CURS_RIGHT:
                slideInDirection(DIR_RIGHT);
                return;

            case 'm':
            case 'M':
            case CH_CURS_DOWN:
                slideInDirection(DIR_DOWN);
                return;

            case CH_ESC:
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
        else if (isGameLost())
            gameLost();
        else
            handleNextEvent();
    }

    return 0;
}
