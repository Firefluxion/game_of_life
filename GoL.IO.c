#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>

#include "GoL.Constants.h"
#include "GoL.Structs.h"
#include "GoL.IO.h"

void write_field_to_file(struct field* field, char filePath[])
{
    FILE *file;
    file = fopen(filePath, "w");
    int x = 0, y = 0;
    for (y = 0; y < field->height; y++)
    {
        for (x = 0; x < field->width; x++)
        {
            struct cell c = get_cell(field, x, y);
            switch(c.state)
            {
            case alive:
                fprintf(file, "%c", c.visual);
                break;
            case dead:
                fprintf(file, " ");
                break;
            default:
                fprintf(file, "X");
                break;
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

struct field* read_field_from_file(char filePath[])
{
    FILE *file;
    file = fopen(filePath, "r");

    int w = get_width(file);
    int h = get_height(file);

    char vis = get_file_visual(file);


    rewind(file);
    // initialize the field with the measures we got
    struct field* f = new_field(w, h, vis, 0);

    int i = 0, filePos = 0;
    for(i = 0, filePos = 0; i < get_field_cell_count(f); i++, filePos++)
    {
        char c = 0;
        fseek(file, filePos, 0);
        fscanf(file, "%c", &c);
        struct cell currentCell = f->cells[i];
        switch(c)
        {
            case '\n':
                filePos++;
                i--;
                break;
            case ' ':
                currentCell.state = dead;
                f->cells[i] = currentCell;
                break;
            default:
                currentCell.state = alive;
                f->cells[i] = currentCell;
                break;
        }
    }
    fclose(file);
    return f;
}

int get_width(FILE* file)
{
    rewind(file);
    char etc = 0;
    int counter = 0;
    int max = 0;
    while (fscanf(file, "%c", &etc) != EOF)
    {
        if(etc == '\n')
        {
            counter = 0;
        }
        else
        {
            counter++;
        }

        if(counter > max)
        {
            max = counter;
        }
    }
    return max;
}

int get_height(FILE* file)
{
    rewind(file);
    int counter = 0;
    char etc = 0;
    while (fscanf(file, "%c", &etc) != EOF)
    {
        if(etc == '\n')
        {
            counter++;
        }
    }
    return counter;
}

char get_file_visual(FILE* file)
{
    rewind(file);
    char ch = 0;


    while (fscanf(file, "%c", &ch) != EOF)
    {
        if (ch != ' ' && ch != '\n')
        {
            return ch;
        }
    }
    return DEFAULTVISUAL;
}
