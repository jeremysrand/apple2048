/*
 * File: ui.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the implementation for the 2048 UI.
 */


#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "game.h"
#include "ui.h"


static bool gPlaySounds = true;


static void quitGame(void)
{
    shutdownGameEngine();
    clrscr();
    exit(0);
}


void playSound(int8_t freq, int16_t duration)
{
    if (!gPlaySounds)
        return;

    while (duration > 0) {
        asm volatile ("STA %w", 0xc030);
        while (freq > 0) {
            freq--;
        }
        duration--;
    }
}


void printInstructions(void)
{
    int seed = 0;

    clrscr();
    printf(
// 0000000001111111111222222222233333333334
// 1234567890123456789012345678901234567890
  "              Apple 2048\n"
  "\n"
  "Use I-J-K-M or the arrow keys to slide\n"
  "all tiles in a direction.  Matching\n"
  "tiles are added together to make a new\n"
  "tile.  On every move, one more tile is\n"
  "added with a random value of either 2\n"
  "or 4.\n"
  "\n"
  "Play ends when all tiles are occupied\n"
  "and no more moves are possible.  Try\n"
  "to get the largest tile you can!\n"
  "\n"
  "Press escape or Q to quit at any time.\n"
  "Press R to start a new game.\n"
  "Press S to toggle sound.\n"
  "Press H to see this info again.\n"
  "\n"
  "\n"
  "\n"
  "       Press any key to start");

    // The amount of time the user waits to read the in
    while (!kbhit())
        seed++;

    cgetc();
    srand(seed);

    clrscr();
}


static void printGrid(void)
{
    tPos x;
    tPos y;

    for (x = 1; x <= BOARD_SIZE; x++) {
        for (y = 1; y <= BOARD_SIZE; y++) {
            textframexy(TILE_X_TO_SCREEN_X(x),
                        TILE_Y_TO_SCREEN_Y(y),
                        TILE_WIDTH,
                        TILE_HEIGHT,
                        TEXTFRAME_WIDE);
        }
    }
}


void printValueAt(uint8_t screenX, uint8_t screenY, char *tileString)
{
    screenX++;
    screenY++;

    cputsxy(screenX, screenY, "        ");
    screenY++;
    cputsxy(screenX, screenY, tileString);
    screenY++;
    cputsxy(screenX, screenY, "        ");
}


static void printValues(void)
{
    tPos x;
    tPos y;

    for (x = 1; x <= BOARD_SIZE; x++) {
        for (y = 1; y <= BOARD_SIZE; y++) {
            printValueAt(TILE_X_TO_SCREEN_X(x), TILE_Y_TO_SCREEN_Y(y),
                    tileStringForPos(x, y));
        }
    }
}


static void printScore(void)
{
    tScore highestTile = highestTarget();

    gotoxy(0,20);
    printf("Score: %-9ld High Score: %-9ld\n", currentScore(), highScore());
    
    if (highestTile == 0) {
        printf("Try to get the %ld tile!\n", nextTarget());
    } else {
        printf("Got %ld!  Now get %ld!\n", highestTile, nextTarget());
    }
}


void printBoard(void)
{
    performAnimations();

    if (kbhit())
        cgetc();

    printGrid();
    printValues();
    printScore();
}


void gameWon(void)
{
    gotoxy(0, 22);
    printf("You have won the game!  Press space...\n");
    while (cgetc() != ' ')
        ;
    clrscr();
    newGame();
}


void gameLost(void)
{
    gotoxy(0, 22);
    printf("Sorry, no more moves.  Press space...");
    while (cgetc() != ' ')
        ;
    clrscr();
    newGame();
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
                quitGame();
                break;

            case 'r':
            case 'R':
                clrscr();
                newGame();
                return;

            case 's':
            case 'S':
                gPlaySounds = !gPlaySounds;
                break;

            case 'h':
            case 'H':
                printInstructions();
                return;
        }
    }
}

