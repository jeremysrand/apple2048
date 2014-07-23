#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


int main(void)
{
    printf("DOES THIS WORK...\n");
    while (!kbhit()) {
    }
    return 0;
}
