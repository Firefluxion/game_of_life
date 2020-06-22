#include "GoL.Runtime.h"

int dead_min = 2;
int dead_max = 3;

int alive_min = 2;
int alive_max = 4;


/*
    Main method for a simulation. Reads user input and displays the iterations
*/
int run_field(struct field* field)
{
    double simulationsPerSecond = 3.0;
    int automatic = 1, finished = 0, i = 0;
    char c = 0;

    //Buffer keeps track of the last n iterations, also compares them to the current one to
    //see if a stable condition has been reached
    struct field* buffer[BUFFERSIZE];

    //Buffer is filled with the current iteration to avoid null references
    for (i = 0; i < BUFFERSIZE; i++)
    {
        buffer[i] = copy_field(field);
    }
    clear();

    do
    {
        if (automatic)
        {
            usleep(1000000 / simulationsPerSecond);
            //Detects id the keyboard was pressed without halting execution
            if (kbhit())
            {
                c = getch();
                fflush(stdin);

                automatic = 0;

                clear();

                print_field(field);
                print_info(field, automatic, simulationsPerSecond, field->iteration, finished);
                continue;
            }
        }
        else
        {
            c = getch();
            //Manual mode; iteration is stopped until the user inputs a key, the info will display the possible actions.
            //Any key will forward the simulation, but some have additional functions
            switch(c)
            {
                // 'a' switches the simulation back to automatic mode
            case 'a':
                automatic = 1;
                clear();
                break;

                // 's' saves the current iteration to a file
            case 's':
                write_field_to_file(field, "Test.txt");
                printf("File saved.\n");
                continue;

                // 'r' reads a saved simulation
            case 'r':
                field = read_field_from_file("Test.txt");
                //The buffer is filled with an empty field to make sure the simulation doesn't stop due to a wrong stable condition
                struct field* empty = new_field(field->width, field->height, ' ', 0);
                for (i = 0; i < BUFFERSIZE; i++)
                {
                    buffer[i] = copy_field(empty);
                }
                clear();

                print_field(field);
                print_info(field, automatic, simulationsPerSecond, field->iteration, finished);
                continue;

                // 'o' gives the user some inputs for the ruleset
            case 'o':
                set_options();
                continue;

                // 'i' allows the user to change the automatic speed
            case 'i':
                printf("Please enter the desired iteration speed.\n");
                simulationsPerSecond = safe_number_input(0, 90);
                break;
            }
        }

        //the current field is copied to the buffer. the modulo makes sure the assigned index stays within the bounds of the array.
        buffer[field->iteration % BUFFERSIZE] = copy_field(field);
        simulate_field(field);

        for (i = 0; i < BUFFERSIZE; i++)
        {
            //the newly simulated field is compared to the buffered values.
            if (compare_fields(buffer[i], field))
            {
                finished = 1;
                clear();
                //iteration is decremented, since no change has occurred since the last iteration.
                field->iteration--;
            }
        }

        print_field(field);
        print_info(field, automatic, simulationsPerSecond, field->iteration, finished);
    }
    // 'x' exits the simulation.
    while (c != 'x' && finished == 0);

    printf("Simulation has completed.");

    return 0;
}

/*
    Simulates the changes in the field for one iteration step with the given ruleset
*/
void simulate_field(struct field* field)
{
    field->iteration++;
    int amount = get_field_cell_count(field);
    struct cell* newCells = new_cell_list(field);
    int i = 0;
    for (i = 0; i < amount; i++)
    {
        newCells[i] = simulate_cell(field, field->cells[i]);
    }
    field->cells = newCells;
}

/*
    Prints the field data itself (including the frame)
*/
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


/*
    Prints the field data utilizing the windows API, currently unused due to problems with small field sizes (e.g. (7, 7))
*/
void print_field_win(struct field* field)
{
    struct cell c;

    HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    const char *message = "";
    if (stdOut != NULL && stdOut != INVALID_HANDLE_VALUE)
    {

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

/*
    Prints the field info next to the field itself
*/
void print_info(struct field* field, int automatic, double iterations, int currentIteration, int finished)
{
    int xDistanceFromField = 3;
    if (!finished)
    {
        if (automatic)
        {
            set_cursor(field->width + xDistanceFromField, 3);
            printf("Simulation is running in auto-mode, press any button to pause.");
            set_cursor(field->width + xDistanceFromField, 5);
            printf("Current iterations per second: %lf", iterations);
        }
        else
        {
            set_cursor(field->width + xDistanceFromField, 3);
            printf("Simulation is in manual mode, press 'a' to return to auto-mode.");
            set_cursor(field->width + xDistanceFromField, 5);
            printf("Press 's' to save the current iteration.");
            set_cursor(field->width + xDistanceFromField, 7);
            printf("Press 'r' to read a saved simulation.");
            set_cursor(field->width + xDistanceFromField, 9);
            printf("Press 'i' to change the automatic iteration speed.");
            set_cursor(field->width + xDistanceFromField, 11);
            printf("Press 'o' to change the ruleset.");
            set_cursor(field->width + xDistanceFromField, 13);
            printf("Press 'x' to end the current simulation.");
        }
    }
    else
    {
        set_cursor(field->width + xDistanceFromField, 0);
        printf("Simulation has reached a stable condition.");
    }
    set_cursor(field->width + xDistanceFromField, 1);
    printf("Current iteration: %d", currentIteration);

    set_cursor(0, field->height + 2);
}

/*
    The cell is simulated by getting its neighbors and checking against the rule set.
*/
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
        if (aliveCount < dead_min || aliveCount > dead_max)
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

/*
    Returns the minimum value if the input is greater than the max value (and vice versa), otherwise returns the input
*/
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

/*
    Code by Christian Wichman in the project "Erweiterte Funktionen der Konsole" (https://moodle.nibis.de/bbs_osb/mod/forum/discuss.php?d=1177)
    Prints an ASCII frame in the given size starting in the top left.
*/
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

/*
    Reads user input for a number, checks whether or not the input is within the set bounds and if it's even a number,
    of not, the user is prompted to reenter the data.
*/
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

/*
    Code by Christian Wichman in the project "Erweiterte Funktionen der Konsole" (https://moodle.nibis.de/bbs_osb/mod/forum/discuss.php?d=1177)
    Sets the cursor to the selected position
*/
int set_cursor(int x, int y)
{
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
    return 0;
}

/*
    Allows the user to input values to modify the rule set.
*/
void set_options()
{
    printf("Please enter a number for the minimum required neighbors for a cell to die. (Default: 2)\n");
    dead_min = safe_number_input(0, (double)INT_MAX);
    printf("Please enter a number for the maximum allowed neighbors for a cell to stay alive. (Default: 3)\n");
    dead_max = safe_number_input(0, (double)INT_MAX);

    printf("Please enter a number for the minimum required neighbors for a cell to revive. (Default: 2)\n");
    alive_min = safe_number_input(0, (double)INT_MAX);
    printf("Please enter a number for the maximum allowed neighbors for a cell to revive. (Default: 4)\n");
    alive_max = safe_number_input(0, (double)INT_MAX);
}

/*
    Code by StackOverflow user 'nbro' (https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c)
    Clears the entire console.
*/
void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
