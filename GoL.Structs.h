#ifndef GOL_STRUCTS_H_INCLUDED
#define GOL_STRUCTS_H_INCLUDED

enum cellState
{
    alive,
    dead
};

struct pos
{
    int x;
    int y;
};

struct cell
{
    struct pos position;
    enum cellState state;
    char visual;
};

struct field
{
    int height;
    int width;

    struct cell* cells;
    int iteration;
};

struct pos* new_pos(int, int);
struct field* new_field(int, int, char, int);
struct cell get_cell(struct field* field, int x, int y);
struct pos* get_neighbor_positions(struct field* field, struct pos position);
struct cell* get_neighbors(struct field* field, struct pos position);
struct cell* new_cell_list(struct field* field);
struct field* copy_field(struct field* origin);
int get_field_cell_count(struct field* field);
int compare_fields(struct field* a, struct field* b);
int get_cell_index(struct field* field, int x, int y);

#include "GoL.Constants.h"
#include "GoL.Runtime.h"

#endif // GOL_STRUCTS_H_INCLUDED
