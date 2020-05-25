#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>

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
struct pos* new_pos(int, int);
struct field* new_field(int, int, char);
void print_field(struct field*);
void print_info(struct field* field, int automatic);
struct cell get_cell(struct field* field, int x, int y);
struct pos* get_neighbor_positions(struct field* field, struct pos position);
struct cell* get_neighbors(struct field* field, struct pos position);
struct cell simulate_cell(struct field* field, struct cell cell);
struct cell* new_cell_list(struct field* field);
void simulate_field(struct field* field);
int set_cursor(int x, int y);
void print_frame(int, int);
float safe_number_input(float min, float max);
void clear();


const int NEIGHBOROFFSETS[2][NEIGHBORCOUNT] = {
    { 1, 1, 0,-1,  -1, -1,  0,  1},
    { 0, 1, 1, 1,   0, -1, -1, -1}
};

int main()
{
    float simulationsPerSecond = 3.0f;
    int automatic = 1;
    char c;

    printf("Please enter the field's width: ");
    int w = safe_number_input(1, (float)INT_MAX);
    printf("Please enter the field's height: ");
    int h = safe_number_input(1, (float)INT_MAX);
    printf("Please a symbol for the field to use: ");
    scanf("%c", &c);
    fflush(stdin);

    struct field* f = new_field(w, h, c);
    c = 0;

    do
    {
        clear();
        print_field(f);
        print_info(f, automatic);
        simulate_field(f);

        if (automatic)
        {
            sleep(1.0f / simulationsPerSecond);
            if (kbhit())
            {
                automatic = 0;
            }
        }
        else
        {
            c = getchar();
        }
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
    set_cursor(0, 0);
    print_frame(field->width + 2, field->height + 2);
    set_cursor(1, 1);
    int x = 0, y = 0;
    for (y = 0; y < field->height; y++)
    {
        set_cursor(1, y + 1);
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
    }
    set_cursor(0, field->height + 2);
};

void print_info(struct field* field, int automatic)
{
    set_cursor(field->width + 2, 1);
    if (automatic)
    {
        printf("Simulation is running in auto-mode, press any button to pause.");
    }
    set_cursor(0, field->height + 2);
}

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

void print_frame(int width, int height)
{
    int i = 0, j = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
                printf("\xC9");
            else if (i == 0 && j == width-1)
                printf("\xBB");
            else if (i == height-1 && j == 0)
                printf("\xC8");
            else if (i == height-1 && j == width-1)
                printf("\xBC");
            else if (i == 0 || i == height-1)
                printf("\xCD");
            else if (j == 0 || j == width-1)
                printf("\xBA");
            else
                printf(" ");
        }
        printf("\n");
    }
}

struct field* new_field(int width, int height, char visual)
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
            f->cells[i].position = *new_pos(x, y);
        }
    }
    return f;
};

struct cell* new_cell_list(struct field* field)
{
    return (struct cell*)malloc(sizeof(struct cell) * field->width * field->height);
};

float safe_number_input(float min, float max)
{
    int valid = 0;
    float n = 0.0f;
    while (!valid)
    {
        if (scanf("%f", &n))
        {
            if (n < min && n < max)
            {
                valid = 0;
                printf("\nThe number must be greater than %f.\n", min);
            }
            else if (n > max)
            {
                valid = 0;
                printf("\nThe number must be less than %f.\n", max);
            }
            else
            {
                valid = 1;
            }
        }
        else
        {
            printf("\nPlease enter a number.\n");
            valid = 0;
        }
        fflush(stdin);
    }
    printf("\n");
    return n;
}

int set_cursor(int x, int y)
{
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
    return 0;
}

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
