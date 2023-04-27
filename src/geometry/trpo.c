#include <func.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char geom[20] = {0};
    char ch;
    int column;

    puts("Enter a geometric shape or q(for exit):");
    while ((ch = getc(stdin)) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    set_info_circle(&circle, &column, stdin);
                    printf("\nResult: \n");
                    show_info_circle(&circle);
                    break;
                } else {
                    print_error(0, ER_NAME);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACK_BRACE);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(stdin)) != '\n');
    }

    return 0;
}
