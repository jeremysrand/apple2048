/*
 * File: apple2048.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the entry point for the 2048 game.
 */

#include <apple2enh.h>
#include <conio.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"


#define TILE_WIDTH 10
#define TILE_HEIGHT 5

#define TILE_X_TO_SCREEN_X(x) (((x) - 1) * TILE_WIDTH)
#define TILE_Y_TO_SCREEN_Y(y) (((y) - 1) * TILE_HEIGHT)


typedef struct tTileAnim
{
    uint8_t fromX;
    uint8_t fromY;
    uint8_t toX;
    uint8_t toY;
    char *tileString;
    char *endTileString;
} tTileAnim;


static tTileAnim gTileAnims[NUM_TILES];
static tDir gAnimDir;
static uint8_t gNumAnims;
static char *gNewTileString = NULL;
static tPos gNewTilePos;

static bool gPlaySounds = true;

void shortDelay(uint16_t howMuch)
{
    while (howMuch > 0) {
        howMuch--;
    }
}


void playSound(int8_t freq, int16_t duration)
{
    while (duration > 0) {
        if (gPlaySounds)
            asm ("STA %w", 0xc030);
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
  "PRESS S TO TOGGLE SOUND.\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "       PRESS ANY KEY TO START");

    // The amount of time the user waits to read the in
    while (!kbhit())
        seed++;

    cgetc();
    srand(seed);

    clrscr();
}


void printGrid(void)
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


void printValues(void)
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


void printScore(void)
{
    gotoxy(0,20);
    printf("CURRENT SCORE: %ld\nTRY TO GET THE %ld TILE!", currentScore(),
            nextTarget());
}


void resetAnimations(void)
{
    memset(gTileAnims, 0, sizeof(gTileAnims));
    gNewTileString = NULL;
    gAnimDir = 0;
    gNumAnims = 0;
}


void performAnimationsLeft(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;

    do {
        animInProgress = false;

        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;
            if ((x % TILE_WIDTH) != (TILE_WIDTH - 1)) {
                x += TILE_WIDTH;
                if (x < 40) {
                    cputcxy(x, y + 1, ' ');
                    cputcxy(x, y + 2, ' ');
                    cputcxy(x, y + 3, ' ');
                }
                x -= TILE_WIDTH;
            }

            x--;

            textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
            
            if (x == tileAnim->toX) {
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                tileAnim->fromX = x;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 2);
        }
    } while (animInProgress);
}


void performAnimationsRight(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;

    do {
        animInProgress = false;

        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;
            if ((x % TILE_WIDTH) != 0) {
                cputcxy(x, y + 1, ' ');
                cputcxy(x, y + 2, ' ');
                cputcxy(x, y + 3, ' ');
            }

            x++;

            textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
            
            if (x == tileAnim->toX) {
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                tileAnim->fromX = x;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 2);
        }
    } while (animInProgress);
}


void performAnimationsUp(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;

    do {
        animInProgress = false;

        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;

            switch ((y % TILE_HEIGHT)) {
                case 0:
                default:
                    cputsxy(x, y + TILE_HEIGHT, "          ");
                    if (y < TILE_Y_TO_SCREEN_Y(BOARD_SIZE))
                        textframexy(x, ((y / TILE_HEIGHT) + 1) * TILE_HEIGHT,
                                TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                    break;
            }

            y--;

            textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
            
            if (y == tileAnim->toY) {
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                tileAnim->fromY = y;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 2);
        }
    } while (animInProgress);
}


void performAnimationsDown(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;

    do {
        animInProgress = false;

        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;

            switch ((y % TILE_HEIGHT)) {
                case 0:
                default:
                    cputsxy(x, y, "          ");
                    textframexy(x, (y / TILE_HEIGHT) * TILE_HEIGHT,
                                TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                    break;
            }

            y++;

            textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
            
            if (y == tileAnim->toY) {
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                tileAnim->fromY = y;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 2);
        }
    } while (animInProgress);
}


void performNewTileAnimation()
{
    uint8_t screenX;
    uint8_t screenY;

    if (gNewTileString == NULL)
        return;

    screenX = TILE_X_TO_SCREEN_X(POS_TO_X(gNewTilePos));
    screenY = TILE_Y_TO_SCREEN_Y(POS_TO_Y(gNewTilePos));

    textframexy(screenX + 2, screenY + 1, TILE_WIDTH - 4, TILE_HEIGHT - 2,
            TEXTFRAME_WIDE);
    shortDelay(500);

    textframexy(screenX + 1, screenY + 1, TILE_WIDTH - 2, TILE_HEIGHT - 1,
            TEXTFRAME_WIDE);
    shortDelay(500);

    textframexy(screenX, screenY, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
    printValueAt(screenX, screenY, gNewTileString);
}


void performAnimations(void)
{
    switch (gAnimDir) {
        case DIR_UP:
            performAnimationsUp();
            break;
        case DIR_DOWN:
            performAnimationsDown();
            break;

        case DIR_LEFT:
            performAnimationsLeft();
            break;

        case DIR_RIGHT:
            performAnimationsRight();
            break;
    }

    performNewTileAnimation();

    resetAnimations();
}


void printBoard(void)
{
    performAnimations();
    printGrid();
    printValues();
    printScore();
}


void gameWon(void)
{
    gotoxy(0, 22);
    printf("YOU HAVE WON THE GAME!  PRESS ANY KEY...\n");
    cgetc();
    clrscr();
    newGame();
}


void gameLost(void)
{
    gotoxy(0, 22);
    printf("SORRY, NO MORE MOVES.  PRESS ANY KEY...");
    cgetc();
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
                exit(0);

            case 'r':
            case 'R':
                clrscr();
                newGame();
                return;

            case 's':
            case 'S':
                gPlaySounds = !gPlaySounds;
                break;
        }
    }
}


void tileMoved(tPos from, tPos to, char *tileString)
{
    tTileAnim *anim = &(gTileAnims[gNumAnims]);
    anim->tileString = tileString;
    anim->endTileString = tileStringForPos(POS_TO_X(to), POS_TO_Y(to));
    anim->fromX = TILE_X_TO_SCREEN_X(POS_TO_X(from));
    anim->fromY = TILE_Y_TO_SCREEN_Y(POS_TO_Y(from));
    anim->toX = TILE_X_TO_SCREEN_X(POS_TO_X(to));
    anim->toY = TILE_Y_TO_SCREEN_Y(POS_TO_Y(to));
    gNumAnims++;
    
    if (anim->fromX > anim->toX) {
        gAnimDir = DIR_LEFT;
    } else if (anim->fromX < anim->toX) {
        gAnimDir = DIR_RIGHT;
    } else if (anim->fromY > anim->toY) {
        gAnimDir = DIR_UP;
    } else if (anim->fromY < anim->toY) {
        gAnimDir = DIR_DOWN;
    }
}


void newTile(tPos at, char *tileString)
{
    gNewTileString = tileString;
    gNewTilePos = at;
}


int main(void)
{
    printInstructions();

    initGameEngine(tileMoved, newTile);
    newGame();

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
