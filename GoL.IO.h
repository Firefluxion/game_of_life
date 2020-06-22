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

void write_field_to_file(struct field* field, char filePath[]);
struct field* read_field_from_file(char filePath[]);
int get_width(FILE*);
int get_height(FILE*);
char get_file_visual(FILE*);


#endif // GOL_IO_H_INCLUDED
