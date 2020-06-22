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

#ifndef GOL_RUNTIME_H_INCLUDED
#define GOL_RUNTIME_H_INCLUDED

extern int dead_min;
extern int dead_max_allowed;

extern int alive_min;
extern int alive_max;

void print_field(struct field*);
void print_field_win(struct field*);
void print_info(struct field* field, int automatic, double iterations, int currentIteration, int finished);
int run_field(struct field* field);
int wrap_around(int n, int min, int max);
struct cell simulate_cell(struct field* field, struct cell cell);
int set_cursor(int x, int y);
void print_frame(int, int);
double safe_number_input(double min, double max);
void set_options();
void clear();




#endif // GOL_RUNTIME_H_INCLUDED
