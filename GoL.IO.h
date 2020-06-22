#ifndef GOL_IO_H_INCLUDED
#define GOL_IO_H_INCLUDED

#include "GoL.Structs.h"

void write_field_to_file(struct field* field, char filePath[]);
struct field* read_field_from_file(char filePath[]);
int get_width(FILE*);
int get_height(FILE*);
char get_file_visual(FILE*);


#endif // GOL_IO_H_INCLUDED
