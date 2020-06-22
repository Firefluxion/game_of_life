#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>

#include "GoL.Constants.h"
#include "GoL.Structs.h"

#ifndef GOL_IO_H_INCLUDED
#define GOL_IO_H_INCLUDED

/*
    Writes a field into a file at the specified file path.
*/
void write_field_to_file(struct field* field, char filePath[]);

/*
    Reads a field from the specified file location.
*/
struct field* read_field_from_file(char filePath[]);

/*
    Returns the width of the field in the specified file.
*/
int get_width(FILE*);

/*
    Returns the height of the field in the specified file.
*/
int get_height(FILE*);

/*
    Returns the visual representations of the cells in the file.
*/
char get_file_visual(FILE*);


#endif
