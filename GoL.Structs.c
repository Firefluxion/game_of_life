#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>

#include "GoL.Structs.h"
#include "GoL.Constants.h"
#include "GoL.Runtime.h"


struct cell get_cell(struct field* field, int x, int y)
{
    struct cell c;
    int i = 0;
    for (i = 0; i < field->height * field->width; i++)
    {
        c = field->cells[i];
        if (c.position.x == x && c.position.y == y)
        {
            return c;
        }
    }
    return c;
};

int get_cell_index(struct field* field, int x, int y)
{
    struct cell cell;
    int i = 0, length = field->height * field->width;
    for (i = 0; i < length; i++)
    {
        cell = field->cells[i];
        if (cell.position.x == x && cell.position.y == y)
        {
            return i;
        }
    }
    return length;
}

struct pos* get_neighbor_positions(struct field* field, struct pos position)
{
    struct pos* positions = (struct pos*)malloc(sizeof(struct pos) * NEIGHBORCOUNT);
    int i = 0, x = 0, y = 0;
    for (i = 0; i < NEIGHBORCOUNT; i++)
    {
        x = wrap_around(NEIGHBOROFFSETS[0][i] + position.x, 0, field->width - 1);
        y = wrap_around(NEIGHBOROFFSETS[1][i] + position.y, 0, field->height - 1);
        positions[i] = *new_pos(x, y);
    }
    return positions;
};

struct cell* get_neighbors(struct field* field, struct pos position)
{
    struct pos* positions = get_neighbor_positions(field, position);
    struct cell* cells = (struct cell*)malloc(sizeof(struct cell) * NEIGHBORCOUNT);

    int i = 0;
    for (i = 0; i < NEIGHBORCOUNT; i++)
    {
        cells[i] = get_cell(field, positions[i].x, positions[i].y);
    }
    return cells;
};

struct pos* new_pos(int x, int y)
{
    struct pos* p = malloc(sizeof(struct pos));
    p->x = x;
    p->y = y;
    return p;
};

struct field* new_field(int width, int height, char visual, int random)
{
    srand(time(0));
    struct field* f = malloc(sizeof(struct field));
    f->width = width;
    f->height = height;
    f->iteration = 0;
    f->cells = new_cell_list(f);

    int x = 0, y = 0, i = 0, r = 0;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++, i++)
        {
            r = rand() % 11;
            f->cells[i].visual = visual;
            f->cells[i].state = random
            ? ((r > 7)
                ? alive
                : dead)
            : dead;
            f->cells[i].position = *new_pos(x, y);
        }
    }
    return f;
};

struct cell* new_cell_list(struct field* field)
{
    return (struct cell*)malloc(sizeof(struct cell) * field->width * field->height);
};

struct field* copy_field(struct field* origin)
{
    char vis = origin->cells[0].visual;
    struct field* newField = new_field(origin->width, origin->height, vis, 0);
    newField->cells = new_cell_list(newField);
    int i = 0, amt = get_field_cell_count(newField);
    for (i = 0; i < amt; i++)
    {
        struct cell newCell;
        struct cell originCell = origin->cells[i];

        newCell.position = originCell.position;
        newCell.visual = originCell.visual;
        newCell.state = originCell.state;

        newField->cells[i] = newCell;
    }
    return newField;
};

int get_field_cell_count(struct field* field)
{
    return field->width * field->height;
}

int compare_fields(struct field* a, struct field* b)
{
    if (a->height != b->height || a->width != b->width)
    {
        return 0;
    }

    int i;
    for (i = 0; i < a->height * a->width; i++)
    {
        if (a->cells[i].state != b->cells[i].state)
        {
            return 0;
        }
    }
    return 1;
};
