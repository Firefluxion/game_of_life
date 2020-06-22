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

extern int dead_min; //Minimum required alive neighbors for a cell to die
extern int dead_max; //Maximum allowed alive neighbors for a cell to stay alive

extern int alive_min; //Minimum required alive neighbors for a cell to revive
extern int alive_max; //Maximum allowed alive neighbors for a cell to stay alive

void print_field(struct field*); // prints field data (including frame)
void print_field_win(struct field*); // prints field data (including frame) utilizing windows API functions (currently unused)
void print_info(struct field* field, int automatic, double iterations, int currentIteration, int finished); //Prints field infos and controls
int run_field(struct field* field); //Starts a simulation and guides the user through it
int wrap_around(int n, int min, int max); //sets a value to max if its less than min and vice versa
struct cell simulate_cell(struct field* field, struct cell cell); //simulates a single iteration for a cell
void simulate_field(struct field* field); //simulates a single iteration for a field
int set_cursor(int x, int y); //sets the console cursor to the designated position
void print_frame(int, int); //prints a frame in the designated size
double safe_number_input(double min, double max); //Checks if user input is within bounds and prompts user to reenter if it isn't
void set_options(); //set rule set
void clear(); //clears the console




#endif // GOL_RUNTIME_H_INCLUDED
