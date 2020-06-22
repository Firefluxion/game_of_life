#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>
#include "GoL.Structs.h"
#include "GoL.IO.h"
#include "GoL.Runtime.h"


/*
    This is the starting point for the game of live program.
*/
int main()
{
    char c;
    printf("Please enter the field's width: ");
    int w = safe_number_input(1, (double)INT_MAX);

    printf("Please enter the field's height: ");
    int h = safe_number_input(1, (double)INT_MAX);

    printf("Please a symbol for the field to use: ");
    scanf("%c", &c);
    fflush(stdin);

    struct field* f = new_field(w, h, c, 1);
    run_field(f);
    return 0;
}




















