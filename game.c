/*
 * File: game.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the implementation of the game logic.
 */


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"


#define POS_IN_DIR(pos, dir) ((pos) + (dir))

// The maximum which the game supports in any one tile is 2^26 because that is
// the largest number which fits in a 8 character tile.  Once this is reached,
// the game is won.  Note this is a very big number so not a huge restriction.
#define MAX_TILE_VALUE 26

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
    " 524288 ",
    "1048576 ",
    "2097152 ",
    "4194304 ",
    "8388608 ",
    "16777216",
    "33554432",
    "67108864"
};

static tScore gValueScores[MAX_TILE_VALUE + 1] = {
    0l, 2l, 4l, 8l, 16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l,
    8192l, 16384l, 32768l, 65536l, 131072l, 262144l, 524288l, 1048576l,
    2097152l, 4194304l, 8388608l, 16777216l, 33554432l, 67108864l };


struct tScoreRecord {
    tScore currentScore;
    tScore highScore;
    tTileValue highestTile;
} gScoreRecord;

static uint8_t gNumEmptyTiles;
static bool gIsGameWon;


void addRandomTile(void);


static tTileMoveCallback gTileMoveCallback = NULL;
static tNewTileCallback gNewTileCallback = NULL;


void initGameEngine(tTileMoveCallback tileMoveCallback,
        tNewTileCallback newTileCallback)
{
    FILE *scoreFile;

    gTileMoveCallback = tileMoveCallback;
    gNewTileCallback = newTileCallback;

    gScoreRecord.highScore = 0;
    gScoreRecord.highestTile = 0;

    scoreFile = fopen("a2048score", "rb");
    if (scoreFile != NULL) {
        fread(&gScoreRecord, sizeof(gScoreRecord), 1, scoreFile);
        fclose(scoreFile);
    }
}


void shutdownGameEngine(void)
{
    FILE *scoreFile;
    scoreFile = fopen("a2048score", "wb");
    if (scoreFile != NULL) {
        fwrite(&gScoreRecord, sizeof(gScoreRecord), 1, scoreFile);
        fclose(scoreFile);
    }
}


void newGame(void)
{
    tPos pos;
    tPos x;
    tPos y;

    gScoreRecord.currentScore = 0;
    gNumEmptyTiles = 0;
    gIsGameWon = false;

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

    addRandomTile();
    addRandomTile();
}


tPos nextPosInDir(tPos pos, tDir dir)
{
    tPos result = pos;
    tPos nextPos;
    tTileValue tileValue = gTileValues[pos];
    tTileValue nextValue;

    while (true) {
        nextPos = POS_IN_DIR(result, dir);
        nextValue = gTileValues[nextPos];

        if ((nextValue != 0) &&
            (nextValue != tileValue))
            break;

        result = nextPos;
        if (nextValue != 0)
            break;
    }
    return result;
}


void increaseScore(tScore value)
{
    gScoreRecord.currentScore += value;
    if (gScoreRecord.currentScore > gScoreRecord.highScore)
        gScoreRecord.highScore = gScoreRecord.currentScore;
}


void updateMaxTile(tTileValue tileValue)
{
    if (gScoreRecord.highestTile < tileValue)
        gScoreRecord.highestTile = tileValue;

    if (tileValue >= MAX_TILE_VALUE)
        gIsGameWon = true;
}


void slideInDirection(tDir dir)
{
    tPos pos;
    tPos destPos;
    int8_t incr;
    tTileValue tileValue;
    bool addNewTile = false;

    if (dir < 0) {
        pos = 0;
        incr = 1;
    } else {
        pos = NUM_TILES - 1;
        incr = -1;
    }

    for ( ; ((pos >= 0) && (pos < NUM_TILES)); pos += incr) {
        if (gTileValues[pos] <= 0)
            continue;
        destPos = nextPosInDir(pos, dir);
        if (destPos == pos)
            continue;
    
        addNewTile = true;

        tileValue = gTileValues[destPos];
        if (tileValue > 0) {
            tileValue++;
            gTileValues[destPos]++;
            gNumEmptyTiles++;
            increaseScore(gValueScores[tileValue]);
            updateMaxTile(tileValue);

            // This is a hack to prevent multiple merges from happening to
            // the same tile in a single turn.  We set the value to a high
            // negative (< -1) and then flip the sign bit later.
            gTileValues[destPos] = -tileValue;
        } else {
            gTileValues[destPos] = gTileValues[pos];
        }
        gTileMoveCallback(pos, destPos, gValueStrings[gTileValues[pos]]);
        gTileValues[pos] = 0; // Empty the old position
    }

    for (pos = 0; pos < NUM_TILES; pos++) {
        tileValue = gTileValues[pos];
        if (tileValue < BLOCKED_TILE_VALUE) {
            gTileValues[pos] = -tileValue;
        }
    }

    if (addNewTile)
        addRandomTile();
}


void addRandomTile(void)
{
    int8_t randTile;
    tPos pos;

    if (gNumEmptyTiles == 0)
        return;

    randTile = (rand() % gNumEmptyTiles);

    for (pos = 0; pos < NUM_TILES; pos++) {
        if (gTileValues[pos] == 0) {
            if (randTile == 0) {
                if (rand() < (RAND_MAX / 10))
                    gTileValues[pos] = 2;   // This creates a 4
                else
                    gTileValues[pos] = 1;   // This creates a 2
                gNumEmptyTiles--;
                gNewTileCallback(pos, gValueStrings[gTileValues[pos]]);
                return;
            }
            randTile--;
        }
    }
}


tScore currentScore(void)
{
    return gScoreRecord.currentScore;
}


tScore highScore(void)
{
    return gScoreRecord.highScore;
}


tScore highestTarget(void)
{
    if (gScoreRecord.highestTile < 11)
        return 0;

    return gValueScores[gScoreRecord.highestTile];
}


tScore nextTarget(void)
{
    tTileValue value = gScoreRecord.highestTile + 1;

    if (value < 11)
        value = 11;

    return gValueScores[value];
}


bool isGameWon(void)
{
    return gIsGameWon;
}


bool isGameLost(void)
{
    tPos x;
    tPos y;
    tPos pos;
    tTileValue tileValue;

    if (gNumEmptyTiles > 0)
        return false;

    for (x = 1; x <= BOARD_SIZE; x++) {
        for (y = 1; y <= BOARD_SIZE; y++) {
            pos = X_Y_TO_POS(x, y);
            tileValue = gTileValues[pos];

            assert(tileValue > 0);

            // If a tile value matches another adjacent tile value, then there
            // are still moves.
            if (tileValue == gTileValues[POS_IN_DIR(pos, DIR_DOWN)])
                return false;
            if (tileValue == gTileValues[POS_IN_DIR(pos, DIR_RIGHT)])
                return false;

            // Because we iterate over all tiles, we only need to check two of
            // the four directions.  That will compare all possible pairs.  It
            // will also check the edge against the blocking tiles but whatever.
        }
    }

    // If we get to here, there were no matching tiles so there are no available
    // moves.
    return true;
}


char *tileStringForPos(tPos x, tPos y)
{
    tTileValue value = gTileValues[X_Y_TO_POS(x, y)];

    if (value < 0)
        value *= -1;

    return gValueStrings[value];
}
