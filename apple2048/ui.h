/*
 * File: ui.h
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the header for the 2048 UI.
 */


#define TILE_WIDTH 10
#define TILE_HEIGHT 5

#define TILE_X_TO_SCREEN_X(x) (((x) - 1) * TILE_WIDTH)
#define TILE_Y_TO_SCREEN_Y(y) (((y) - 1) * TILE_HEIGHT)


extern void printInstructions(void);

extern void printBoard(void);

extern void gameWon(void);

extern void gameLost(void);

extern void handleNextEvent(void);

extern void printValueAt(uint8_t screenX, uint8_t screenY, char *tileString);

extern void playSound(int8_t freq, int16_t duration);
