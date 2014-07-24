/*
 * File: game.h
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file has the external definitions for the game interface.
 */


#include <stdint.h>
#include <stdbool.h>


#define BOARD_SIZE 4

// A 4x4 board is actually represented as a 6x6 board internally.  This gives
// us special tiles around the whole board which can have a value which
// prevents tiles from scrolling beyond the bounds of the board.  Also, this
// is why the values of DIR_DOWN and DIR_UP in the game.h file is a bit
// strange.
#define NUM_TILES ((BOARD_SIZE + 2) * (BOARD_SIZE + 2))

#define POS_TO_X(pos) ((pos) % (BOARD_SIZE + 2))
#define POS_TO_Y(pos) ((pos) / (BOARD_SIZE + 2))
#define X_Y_TO_POS(x, y) (((y) * (BOARD_SIZE + 2)) + (x))

#define DIR_DOWN (BOARD_SIZE + 2)
#define DIR_UP (-(DIR_DOWN))
#define DIR_RIGHT 1
#define DIR_LEFT -1


typedef int8_t tDir;
typedef int8_t tPos;
typedef uint32_t tScore;

typedef void (*tTileMoveCallback)(tPos from, tPos to, char *tileString);
typedef void (*tNewTileCallback)(tPos at, char *tileString);

extern void initGameEngine(tTileMoveCallback tileMoveCallback,
        tNewTileCallback newTileCallback);

extern void newGame(void);

extern void slideInDirection(tDir dir);

extern tScore currentScore(void);

extern tScore nextTarget(void);

extern bool isGameWon(void);

extern bool isGameLost(void);

// Positions are 1 based so the top-left corner is (1, 1) and the bottom-right
// corner is (BOARD_SIZE, BOARD_SIZE).
extern char *tileStringForPos(tPos x, tPos y);
