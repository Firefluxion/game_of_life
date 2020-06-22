#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>

#include "GoL.Structs.h"
#include "GoL.Constants.h"
#include "GoL.IO.h"
#include "GoL.Runtime.h"

int dead_min = 2;
int dead_max_allowed = 3;

int alive_min = 2;
int alive_max = 4;

int run_field(struct field* field)
{
    double simulationsPerSecond = 3.0;
    int automatic = 1, finished = 0, i = 0;
    char c;
    struct field* buffer[BUFFERSIZE]; //Kommentar

    for (i = 0; i < BUFFERSIZE; i++)
    {
        buffer[i] = copy_field(field);
    }

    c = 0;
    clear();

    do
    {
        if (automatic)
        {
            usleep(1000000 / simulationsPerSecond);
            if (kbhit())
            {
                automatic = 0;
                clear();
                print_field(field);
                print_info(field, automatic, simulationsPerSecond, field->iteration, finished);
                fflush(stdin);
                continue;
            }
        }
        else
        {
            switch(getch())
            {
            case 'a':
                automatic = 1;
                fflush(stdin);
                break;

            case 's':
                write_field_to_file(field, "Test.txt");
                printf("File saved.\n");
                continue;

            case 'r':
                field = read_field_from_file("Test.txt");
                struct field* empty = new_field(field->width, field->height, ' ', 0);
                for (i = 0; i < BUFFERSIZE; i++)
                {
                    buffer[i] = empty;
                }
                clear();
                print_field(field);
                print_info(field, automatic, simulationsPerSecond, field->iteration, finished);
                continue;

            case 'o':
                set_options();
                continue;

            case 'i':
                printf("Please enter the desired iteration speed.\n");
                simulationsPerSecond = safe_number_input(0, 90);
                break;
            }
        }

        buffer[field->iteration % BUFFERSIZE] = copy_field(field);
        simulate_field(field);

        for (i = 0; i < BUFFERSIZE; i++)
        {
            if (compare_fields(buffer[i], field))
            {
                finished = 1;
                clear();
                field->iteration--;
            }
        }

        print_field(field);
        print_info(field, automatic, simulationsPerSecond, field->iteration, finished);
    }
    while (c != 'x' && finished == 0);

    printf("Simulation has completed.");

    return 0;
}

void simulate_field(struct field* field)
{
    field->iteration++;
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

void print_field_win(struct field* field)
{
    struct cell c;

    HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    const char *message = "hello world";
    if (stdOut != NULL && stdOut != INVALID_HANDLE_VALUE)
    {

    //    WriteConsoleA(stdOut, message, strlen(message), &written, NULL);
    }

    set_cursor(0, 0);
    print_frame(field->width + 2, field->height + 2);
    set_cursor(1, 1);
    int x = 0, y = 0;
    for (y = 0; y < field->height; y++)
    {
        for (x = 0; x < field->width; x++)
        {
            set_cursor(1 + x, 1 + y);
            c = get_cell(field, x, y);
            switch(c.state)
            {
            case alive:
                message = &c.visual;
                break;
            case dead:
                message = " ";
                break;
            default:
                message = "X";
                break;
            }
            WriteConsoleA(stdOut, message, strlen(message), &written, NULL);
        }
    }
    set_cursor(0, field->height + 2);
}

void print_info(struct field* field, int automatic, double iterations, int currentIteration, int finished)
{
    if (!finished)
    {
        if (automatic)
        {
            set_cursor(field->width + 2, 1);
            printf("Simulation is running in auto-mode, press any button to pause.");
            set_cursor(field->width + 2, 3);
            printf("Current iterations per second: %lf", iterations);
        }
        else
        {
            set_cursor(field->width + 2, 1);
            printf("Simulation is in manual mode, enter 'a' to return to auto-mode.");
        }
    }
    else
    {
        set_cursor(field->width + 2, 1);
        printf("Simulation has reached a stable condition.");
    }
    set_cursor(field->width + 2, 5);
    printf("Current iteration: %d", currentIteration);

    set_cursor(0, field->height + 2);
}
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
        if (aliveCount < dead_min || aliveCount > dead_max_allowed)
        {
            cell.state = dead;
        }
        break;
    case dead:
        if (aliveCount < alive_max && aliveCount > alive_min)
        {
            cell.state = alive;
        }
        break;
    }

    return cell;
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

double safe_number_input(double min, double max)
{
    int valid = 0;
    double n = 0.0;
    while (!valid)
    {
        if (scanf("%lf", &n))
        {
            if (n < min && n < max)
            {
                valid = 0;
                printf("\nThe number must be greater than %lf.\n", min);
            }
            else if (n > max)
            {
                valid = 0;
                printf("\nThe number must be less than %lf.\n", max);
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

void set_options()
{
    printf("Please enter a number for the minimum required neighbors for a cell to die.\n");
    dead_min = safe_number_input(0, (double)INT_MAX);
    printf("Please enter a number for the maximum allowed neighbors for a cell to stay alive.\n");
    dead_max_allowed = safe_number_input(0, (double)INT_MAX);

    printf("Please enter a number for the minimum required neighbors for a cell to revive.\n");
    alive_min = safe_number_input(0, (double)INT_MAX);
    printf("Please enter a number for the maximum allowed neighbors for a cell to revive.\n");
    alive_max = safe_number_input(0, (double)INT_MAX);
}


void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
