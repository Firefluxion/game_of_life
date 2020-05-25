#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NEIGHBORCOUNT 8
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
};

int wrap_around(int n, int min, int max);
struct pos* newPos(int, int);
struct field* newField(int, int, char);
void print_field(struct field*);
struct cell get_cell(struct field* field, int x, int y);
struct pos* get_neighbor_positions(struct field* field, struct pos position);
struct cell* get_neighbors(struct field* field, struct pos position);
struct cell simulate_cell(struct field* field, struct cell cell);
struct cell* new_cell_list(struct field* field);
void simulate_field(struct field* field);
void clear();

const int NEIGHBOROFFSETS[2][NEIGHBORCOUNT] = {
    { 1, 1, 0,-1,  -1, -1,  0,  1},
    { 0, 1, 1, 1,   0, -1, -1, -1}
};

int main()
{
    char c;
    struct field* f = newField(40, 20, '#');
    do
    {
        clear();
        print_field(f);
        simulate_field(f);
        c = getchar();
    }
    while (c != 'x');

    return 0;
}

void simulate_field(struct field* field)
{
    int amount = field->width * field->height;
    struct cell* newCells = new_cell_list(field);
    int i = 0;
    for (i = 0; i < amount; i++)
    {
        newCells[i] = simulate_cell(field, field->cells[i]);
    }
    field->cells = newCells;
}

void print_field(struct field* field)
{
    int x = 0, y = 0;
    for (y = 0; y < field->height; y++)
    {
        for (x = 0; x < field->width; x++)
        {
            struct cell c = get_cell(field, x, y);
            switch(c.state)
            {
            case alive:
                printf("%c", c.visual);
                break;
            case dead:
                printf(" ");
                break;
            default:
                printf("X");
                break;
            }
        }
    printf("\n");
    }
};

struct cell get_cell(struct field* field, int x, int y)
{
    struct cell c;
    int i = 0;
    for (i; i < field->height * field->width; i++)
    {
        c = field->cells[i];
        if (c.position.x == x && c.position.y == y)
        {
            return c;
        }
    }
    return;
};

struct cell simulate_cell(struct field* field, struct cell cell)
{
    struct cell* neighbors = get_neighbors(field, cell.position);

    int i = 0, aliveCount = 0;
    for (i = 0; i < NEIGHBORCOUNT; i++)
    {
        struct cell c = neighbors[i];
        if (c.state == alive)
        {
            aliveCount++;
        }
    }
    switch (cell.state)
    {
    case alive:
        if (aliveCount < 2 || aliveCount > 3)
        {
            cell.state = dead;
        }
        break;
    case dead:
        if (aliveCount == 3)
        {
            cell.state = alive;
        }
        break;
    }

    return cell;
};

struct pos* get_neighbor_positions(struct field* field, struct pos position)
{
    struct pos* positions = (struct pos*)malloc(sizeof(struct pos) * NEIGHBORCOUNT);
    int i = 0, x = 0, y = 0;
    for (i = 0; i < NEIGHBORCOUNT; i++)
    {
        x = wrap_around(NEIGHBOROFFSETS[0][i] + position.x, 0, field->width - 1);
        y = wrap_around(NEIGHBOROFFSETS[1][i] + position.y, 0, field->height - 1);
        positions[i] = *newPos(x, y);
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

struct pos* newPos(int x, int y)
{
    struct pos* p = malloc(sizeof(struct pos));
    p->x = x;
    p->y = y;
    return p;
};

int wrap_around(int n, int min, int max)
{
    if (n < min)
    {
        return max;
    }
    else if (n > max)
    {
        return min;
    }
    return n;
}

struct field* newField(int width, int height, char visual)
{
    srand(time(0));
    struct field* f = malloc(sizeof(struct field));
    f->width = width;
    f->height = height;
    f->cells = new_cell_list(f);

    int x = 0, y = 0, i = 0, r = 0;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++, i++)
        {
            r = rand() % 11;
            f->cells[i].visual = visual;
            f->cells[i].state = (r > 7)
                ? alive
                : dead;
            f->cells[i].position = *newPos(x, y);
        }
    }
    return f;
    //return (struct pos*)malloc(sizeof(struct pos) * size);
};

struct cell* new_cell_list(struct field* field)
{
    return (struct cell*)malloc(sizeof(struct cell) * field->width * field->height);
};

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
