#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>

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
void simulate_field(struct field* field);
int get_cell_index(struct field* field, int x, int y);


#endif // GOL_STRUCTS_H_INCLUDED
