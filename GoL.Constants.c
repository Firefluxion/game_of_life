#include "GoL.Constants.h"

/*
    Relative coordinates for a given position.
*/
const int NEIGHBOROFFSETS[2][NEIGHBORCOUNT] = {
    { 1, 1, 0,-1,  -1, -1,  0,  1},
    { 0, 1, 1, 1,   0, -1, -1, -1}
};

/*
    This is the default char that we use for usually living cells.
*/
const char DEFAULTVISUAL = 'X';
