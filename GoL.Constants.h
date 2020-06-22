#include <stdio.h>
#include <stdlib.h>

#ifndef GOL_CONSTANTS_H_INCLUDED
#define GOL_CONSTANTS_H_INCLUDED

#define NEIGHBORCOUNT 8
#define BUFFERSIZE 40

/*
    Defines the relative coordinates for a given position.
*/
extern const int NEIGHBOROFFSETS[2][NEIGHBORCOUNT];

/*
    Defines the visual default representation usually for the cell.
*/
extern const char DEFAULTVISUAL;

#endif
