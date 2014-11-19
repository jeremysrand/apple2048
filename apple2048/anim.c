/*
 * File: anim.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file is the implementation file the 2048 animations.
 */


#include <apple2enh.h>
#include <conio.h>
#include <stddef.h>
#include <string.h>

#include "game.h"
#include "textpage.h"
#include "ui.h"


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

static void (*gSwitchToPage2)(void) = switchToPage2;


static void shortDelay(uint16_t howMuch)
{
    while (howMuch > 0) {
        howMuch--;
    }
}


static void resetAnimations(void)
{
    memset(gTileAnims, 0, sizeof(gTileAnims));
    gNewTileString = NULL;
    gAnimDir = 0;
    gNumAnims = 0;
}


static void performAnimationsLeft(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;
    bool firstFrame = true;
    uint8_t speed = 1;

    if (gNumAnims > BOARD_SIZE)
        speed = 2;

    do {
        animInProgress = false;

        gSwitchToPage2();
        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY; 

            if (!firstFrame) {
                if (speed == 1) {
                    switch (x % TILE_WIDTH) {
                        case 0:
                            break;
                        case 1:
                            textframexy(((x / TILE_WIDTH) + 1) * TILE_WIDTH, y,
                                    TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                            break;
                        default:
                            x += TILE_WIDTH - 1;
                            cputcxy(x, y + 1, ' ');
                            cputcxy(x, y + 2, ' ');
                            cputcxy(x, y + 3, ' ');
                            x -= TILE_WIDTH - 1;
                            break;
                    } 
                } else {
                    switch (x % TILE_WIDTH) {
                        case 0:
                            break;
                        case 1:
                        case 2:
                            x += TILE_WIDTH - 1;
                            cputcxy(x, y + 1, ' ');
                            cputcxy(x, y + 2, ' ');
                            cputcxy(x, y + 3, ' ');
                            x -= TILE_WIDTH - 1;
                            textframexy(((x / TILE_WIDTH) + 1) * TILE_WIDTH, y,
                                    TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                            break;
                        default:
                            x += TILE_WIDTH - 2;
                            cputsxy(x, y + 1, "  ");
                            cputsxy(x, y + 2, "  ");
                            cputsxy(x, y + 3, "  ");
                            x -= TILE_WIDTH - 2;
                            break;
                    } 
                }
            }

            x -= speed;

            
            if (x <= tileAnim->toX) {
                x = tileAnim->toX;
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->fromX = x;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 1);
        }
        switchToPage1();
        firstFrame = false;
    } while (animInProgress);
}


static void performAnimationsRight(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;
    bool firstFrame = true;
    uint8_t speed = 1;

    if (gNumAnims > BOARD_SIZE)
        speed = 2;

    do {
        animInProgress = false;

        gSwitchToPage2();
        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;

            if (!firstFrame) {
                if (speed == 1) {
                    switch (x % TILE_WIDTH) {
                        case 0:
                            break;
                        case (TILE_WIDTH - 1):
                            textframexy((x / TILE_WIDTH) * TILE_WIDTH, y,
                                    TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                            break;
                        default:
                            cputcxy(x, y + 1, ' ');
                            cputcxy(x, y + 2, ' ');
                            cputcxy(x, y + 3, ' ');
                            break;
                    }
                } else {
                    switch (x % TILE_WIDTH) {
                        case 0:
                            break;
                        case (TILE_WIDTH - 1):
                        case (TILE_WIDTH - 2):
                            textframexy((x / TILE_WIDTH) * TILE_WIDTH, y,
                                    TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                            cputcxy(x, y + 1, ' ');
                            cputcxy(x, y + 2, ' ');
                            cputcxy(x, y + 3, ' ');
                            break;
                        default:
                            cputsxy(x, y + 1, "  ");
                            cputsxy(x, y + 2, "  ");
                            cputsxy(x, y + 3, "  ");
                            break;
                    }
                }
            }

            x += speed;

            
            if (x >= tileAnim->toX) {
                x = tileAnim->toX;
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->fromX = x;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 1);
        }
        switchToPage1();
        firstFrame = false;
    } while (animInProgress);
}


static void performAnimationsUp(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;
    bool firstFrame = true;
    uint8_t speed = 1;

    if (gNumAnims > BOARD_SIZE)
        speed = 2;

    do {
        animInProgress = false;

        gSwitchToPage2();
        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;

            if (!firstFrame) {
                switch ((y % TILE_HEIGHT)) {
                    case 0:
                        break;
                    default:
                        cputsxy(x + 1, y + TILE_HEIGHT - 1, "        ");
                        if (speed > 1)
                            cputsxy(x + 1, y + TILE_HEIGHT - 1, "        ");
                        textframexy(x, ((y / TILE_HEIGHT) + 1) * TILE_HEIGHT,
                                TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                        break;
                }
            }

            if (y < speed)
                y = 0;
            else
                y -= speed;;
            
            if (y <= tileAnim->toY) {
                y = tileAnim->toY;
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->fromY = y;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 1);
        }
        switchToPage1();
        firstFrame = false;
    } while (animInProgress);
}


static void performAnimationsDown(void)
{
    bool animInProgress;
    tPos pos;
    tTileAnim *tileAnim;
    tPos x;
    tPos y;
    bool firstFrame = true;
    uint8_t speed = 1;

    if (gNumAnims > BOARD_SIZE)
        speed = 2;

    do {
        animInProgress = false;

        gSwitchToPage2();
        for (pos = 0; pos < gNumAnims; pos++) {
            tileAnim = &(gTileAnims[pos]);
            if (tileAnim->tileString == NULL)
                continue;

            x = tileAnim->fromX;
            y = tileAnim->fromY;

            if (!firstFrame) {
                switch ((y % TILE_HEIGHT)) {
                    case 0:
                        break;
                    default:
                        cputsxy(x + 1, y, "        ");
                        if (speed > 1)
                            cputsxy(x + 1, y + 1, "        ");
                        textframexy(x, (y / TILE_HEIGHT) * TILE_HEIGHT,
                                    TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                        break;
                }
            }

            y += speed;

            
            if (y >= tileAnim->toY) {
                y = tileAnim->toY;
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->tileString = NULL;
                printValueAt(x, y, tileAnim->endTileString);
            } else {
                textframexy(x, y, TILE_WIDTH, TILE_HEIGHT, TEXTFRAME_WIDE);
                tileAnim->fromY = y;
                animInProgress = true;
                printValueAt(x, y, tileAnim->tileString);
            }
            playSound(200, 1);
        }
        switchToPage1();
        firstFrame = false;
    } while (animInProgress);
}


static void performNewTileAnimation()
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


static void tileMoved(tPos from, tPos to, char *tileString)
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


static void newTile(tPos at, char *tileString)
{
    gNewTileString = tileString;
    gNewTilePos = at;
}


void initAnimations(void)
{
    switch (get_ostype()) {
        case APPLE_IIGS:
        case APPLE_IIGS1:
        case APPLE_IIGS3:
            gSwitchToPage2 = gsSwitchToPage2;
            break;
    }

    initGameEngine(tileMoved, newTile);
}
    

