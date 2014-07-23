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

#define DIR_DOWN (BOARD_SIZE + 2)
#define DIR_UP (-(DIR_DOWN))
#define DIR_RIGHT 1
#define DIR_LEFT -1


typedef int8_t tDir;
typedef int8_t tPos;
typedef uint32_t tScore;


void initGame(void);

void slideInDirection(tDir dir);

tScore currentScore(void);

tScore nextTarget(void);

bool isGameWon(void);

bool isGameLost(void);

// Positions are 1 based so the top-left corner is (1, 1) and the bottom-right
// corner is (BOARD_SIZE, BOARD_SIZE).
char *tileStringForPos(tPos x, tPos y);
