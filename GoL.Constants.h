#ifndef GOL_CONSTANTS_H_INCLUDED
#define GOL_CONSTANTS_H_INCLUDED

#include <stdio.h>

// The amount of neighbours a single cell has
#define NEIGHBORCOUNT 8
// buffer size for checking for a stable condition
#define BUFFERSIZE 40
// buffer size for save names
#define SAVEBUFFER 50

// Min and Max macros
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/*
    Defines the relative coordinates for a given position.
*/
extern const int NEIGHBOROFFSETS[2][NEIGHBORCOUNT];

/*
    Defines the visual default representation usually for the cell.
*/
extern const char DEFAULTVISUAL;

#endif
