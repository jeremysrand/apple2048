#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"


void printBoard(void)
{
    tPos x;
    tPos y;

    clrscr();

    for (y = 1; y <= BOARD_SIZE; y++) {
        printf("+");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("--------+");
        }
        printf("\n!");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("        !");
        }
        printf("\n!");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("%s!", tileStringForPos(x, y));
        }
        printf("\n!");
        for (x = 1; x <= BOARD_SIZE; x++) {
            printf("        !");
        }
        printf("\n");
    }
    printf("+");
    for (x = 1; x <= BOARD_SIZE; x++) {
        printf("--------+");
    }
    printf("\n\nCURRENT SCORE: %ld\nTRY TO GET THE %ld TILE!", currentScore(),
            nextTarget());
}


void gameWon(void)
{
    printf("\nCONGRATULATIONS, YOU HAVE WON THE GAME!\n");
    cgetc();
    exit(0);
}


void gameLost(void)
{
    printf("\nSORRY, NO MORE MOVES.\n");
    cgetc();
    exit(0);
}


void handleNextEvent(void)
{
    char ch;

    while (true) {
        ch = cgetc();
        switch (ch) {
            case 'i':
            case 'I':
            case 11:        // Up arrow
                slideInDirection(DIR_UP);
                return;

            case 'j':
            case 'J':
            case 8:         // Left arrow
                slideInDirection(DIR_LEFT);
                return;

            case 'k':
            case 'K':
            case 21:        // Right arrow
                slideInDirection(DIR_RIGHT);
                return;

            case 'm':
            case 'M':
            case 10:        // Up arrow
                slideInDirection(DIR_DOWN);
                return;

            case 27:        // Escape
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
    initGame();

    while (true) {
        printBoard();

        if (isGameWon())
            gameWon();

        if (isGameLost())
            gameLost();

        handleNextEvent();
    }

    return 0;
}
