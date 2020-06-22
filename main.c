#include "GoL.h"

int main()
{
    printf("Please enter the field's width: ");
    int w = safe_number_input(1, (double)INT_MAX);

    printf("Please enter the field's height: ");
    int h = safe_number_input(1, (double)INT_MAX);

    printf("Please a symbol for the field to use: ");
    char c = 0;
    scanf("%c", &c);
    fflush(stdin);


    struct field* f = new_field(w, h, c, 1);
    run_field(f);
    return 0;
}




















