/**

    The "Game of Life"
    Original concept by John Horton Conway

    Authors: Alexander Thielker & Robin Gerwe
    Date: 22.06.2020

    The Game of Life is a cellular automaton. It is a "zero-player game", and is made out of a potentially
    infinite grid of cells. These cells can be either "dead" or "alive". A cell may die or be revived
    depending on the amount of living neighbors.
    The initial pattern is determined randomly.
    (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
    (https://github.com/Firefluxion/game_of_life)
**/

#include "GoL.h"


/*
    This is the starting point for the game of live program.
    This is an example of the default implementation.
*/
int main()
{
    printf("Please enter the field's width: ");
    int w = safe_number_input(1, (double)INT_MAX);

    printf("Please enter the field's height: ");
    int h = safe_number_input(1, (double)INT_MAX);

    printf("Please a symbol for the field to use: ");
    char c = 0;
    scanf("%c", &c);
    fflush(stdin);

    struct field* f = new_field(w, h, c, 1);
    run_field(f);
    return 0;
}




















