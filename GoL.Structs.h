#ifndef GOL_STRUCTS_H_INCLUDED
#define GOL_STRUCTS_H_INCLUDED

/*
    This represents is the cell state for a single cell.
*/
enum cellState
{
    alive,
    dead
};

/*
    The position encapsulates the position in the grid of cells
*/
struct pos
{
    int x;
    int y;
};

/*
    The cell represents a cell with a 2d position a state and the visual representation that is displayed on the grid.
*/
struct cell
{
    struct pos position;
    enum cellState state;
    char visual;
};

/*
    The field represents the 2d playing field for the game of life with its cells and the current iteration.
*/
struct field
{
    int height;
    int width;

    struct cell* cells;
    int iteration;
};

/*
    Returns the represented position for the given coordinates.
*/
struct pos* new_pos(int, int);

/*
    Creates a new fields with the given arguments.
    The last int is to control the randomness.
*/
struct field* new_field(int, int, char, int);

/*
    Returns a cell in the given field from the specified position.
*/
struct cell get_cell(struct field* field, int x, int y);

/*
    Gets the positions of the neighbors.
*/
struct pos* get_neighbor_positions(struct field* field, struct pos position);

/*
    Returns a list of the neighbor cells from the specified position.
*/
struct cell* get_neighbors(struct field* field, struct pos position);

/*
    Creates a new cell list for the given field.
*/
struct cell* new_cell_list(struct field* field);

/*
    Returns a copie of the given field.
*/
struct field* copy_field(struct field* origin);

/*
    Returns the amount of cells in the given field.
*/
int get_field_cell_count(struct field* field);
/*
    Compares two field and returns 1 if they are equal and 0 if they are no equal.
*/
int compare_fields(struct field* a, struct field* b);
/*
    Returns the amount of cells in the position of the specified field.
*/
int get_cell_index(struct field* field, int x, int y);

#include "GoL.Constants.h"
#include "GoL.Runtime.h"

#endif // GOL_STRUCTS_H_INCLUDED
