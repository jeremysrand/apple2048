/*
 * File: game.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the implementation of the game logic.
 */


#include "game.h"


// A 4x4 board is actually represented as a 6x6 board internally.  This gives
// us special tiles around the whole board which can have a value which
// prevents tiles from scrolling beyond the bounds of the board.  Also, this
// is why the values of DIR_DOWN and DIR_UP in the game.h file is a bit
// strange.
#define NUM_TILES ((BOARD_SIZE + 2) * (BOARD_SIZE + 2))

#define POS_TO_X(pos) ((pos) % (BOARD_SIZE + 2))
#define POS_TO_Y(pos) ((pos) / (BOARD_SIZE + 2))
#define X_Y_TO_POS(x, y) (((y) * (BOARD_SIZE + 2)) + (x))

// The maximum which the game supports in any one tile is 2^19 because that is
// the largest number which fits in a 6 character tile.  Once this is reached,
// the game is won.  Note this is a very big number so not a huge restriction.
#define MAX_TILE_VALUE 19

#define BLOCKED_TILE_VALUE -1

// A tile value is stored as an exponent of base 2.  So:
//      - 0 is an empty tile
//      - 1 is a 2 (2^1)
//      - 2 is a 4 (2^2)
//      - 3 is a 8 (2^3)
//      ... etc ...
// Also, the special value -1 is used to create tiles around the edge of the
// board which ensures tiles don't go beyond the bounds of the board.
typedef int8_t tTileValue;


static tTileValue gTileValues[NUM_TILES];

static char *gValueStrings[MAX_TILE_VALUE + 1] = {
    "        ",  // 0 or empty tile
    "    2   ",
    "    4   ",
    "    8   ",
    "   16   ",
    "   32   ",
    "   64   ",
    "   128  ",
    "   256  ",
    "   512  ",
    "  1024  ",
    "  2048  ",
    "  4096  ",
    "  8192  ",
    "  16384 ",
    "  32768 ",
    "  65536 ",
    " 131072 ",
    " 262144 ",
    " 524288 "
};

static tScore gValueScores[MAX_TILE_VALUE + 1] = {
    0l, 2l, 4l, 8l, 16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l,
    8192l, 16384l, 32768l, 65536l, 131072l, 262144l, 524288l };

static tScore gCurrentScore;
static tTileValue gNextTarget = 11;
static uint8_t gNumEmptyTiles;


void initGame(void)
{
    tPos pos;
    tPos x;
    tPos y;

    gCurrentScore = 0;
    gNumEmptyTiles = 0;

    for (pos = 0; pos < NUM_TILES; pos++) {
        x = POS_TO_X(pos);
        y = POS_TO_Y(pos);

        if ((x == 0) ||
            (x > BOARD_SIZE) ||
            (y == 0) ||
            (y > BOARD_SIZE)) {
            gTileValues[pos] = BLOCKED_TILE_VALUE;
        } else {
            gTileValues[pos] = 0;
            gNumEmptyTiles++;
        }
    }
}


void slideInDirection(tDir dir)
{
}


tScore currentScore(void)
{
    return gCurrentScore;
}


tScore nextTarget(void)
{
    return gValueScores[gNextTarget];
}


bool isGameWon(void)
{
    return (gNextTarget > MAX_TILE_VALUE);
}


bool isGameLost(void)
{
    return (gNumEmptyTiles == 0);
}


char *tileStringForPos(tPos x, tPos y)
{
    return gValueStrings[gTileValues[X_Y_TO_POS(x, y)]];
}
