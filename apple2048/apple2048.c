/*
 * File: apple2048.c
 * Author: Jeremy Rand
 * Date: July 23, 2014
 *
 * This file contains the entry point for the 2048 game.
 */


#include "anim.h"
#include "game.h"
#include "ui.h"


int main(void)
{
    initAnimations();
    printInstructions();

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
