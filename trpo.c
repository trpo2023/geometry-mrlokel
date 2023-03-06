#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 20

typedef struct point {
    double x;
    double y;
} Point;

typedef struct circle {
    Point point;
    double radius;
    double perimeter;
    double area;
} Circle;

enum Errors {
    ER_NAME,
    ER_NOT_DOUBLE,
    ER_BACK_BRACE,
    ER_UNEXPECT_TOKEN,
    ER_EXPECT_COMMA,
    ER_UNEXPECT_COMMA,
};

void print_error(int column, int status)
{
    for (int i = 0; i < column; i++) {
        putchar(' ');
    }
    printf("^\n");
    switch (status) {
    case ER_NAME:
        printf("Error at column %d: expected "
               "'circle'\n",
               column);
        break;
    case ER_NOT_DOUBLE:
        printf("Error at column %d: expected "
               "'<double>'\n",
               column);
        break;
    case ER_BACK_BRACE:
        printf("Error at column %d: expected ')'\n",
               column);
        break;
    case ER_UNEXPECT_TOKEN:
        printf("Error at column %d: expected "
               "token\n",
               column);
        break;
    case ER_EXPECT_COMMA:
        printf("Error at column %d: expected ','\n",
                column);
        break;
    case ER_UNEXPECT_COMMA:
        printf("Error at column %d: expected ','\n",
               column);
    }
}

void del_space(int* column, FILE* scan)
{
    char ch;
    while ((ch = getc(scan)) == ' ') {
        *column += 1;
        continue;
    }
    if (ch != ' ')
        ungetc(ch, scan);
}

double get_number(int* column, FILE* scan)
{
    char temp[20];
    char ch;
    int point_count = 0;
    int i = 0;
    int minus_count = 0;

    del_space(column, scan);

    while ((ch = getc(scan)) != ' ') {
        temp[i] = ch;

        if (temp[i] == '.') {
            point_count++;
            if (point_count > 1) {
                print_error(*column + i + 1, ER_NOT_DOUBLE);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == '-') {
            minus_count++;
            if (minus_count > 1) {
                print_error(*column + i + 1, ER_NOT_DOUBLE);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == ')') {
            ungetc(temp[i], scan);
            i++;
            break;
        }

        if (temp[i] == ',') {
            ungetc(temp[i], scan);
            break;
        }

        if (temp[i] == '(') {
            i++;
            print_error(*column + i, ER_BACK_BRACE);
            exit(EXIT_FAILURE);
        }

        if (!isdigit(temp[i]) && temp[i] != '.' && temp[i] != '-') {
            i++;
            print_error(*column + i, ER_NOT_DOUBLE);
            exit(EXIT_FAILURE);
        }

        i++;
    }
    del_space(column, scan);
    *column += i + 1;
    char* eptr;
    return strtod(temp, &eptr);
}

bool expect(char expect, int* column, int status, FILE* scan)
{
    char ch;
    if ((ch = getc(scan)) == expect) {
        return true;
    } else {
            print_error(*column, status);
            exit(EXIT_FAILURE);
    }
}

bool unexpect(char unexpect, int* column, int status, FILE* scan)
{
    char ch;
    if ((ch = getc(scan)) == unexpect) {
        print_error(*column, status);
        exit(EXIT_FAILURE);
    }
    ungetc(ch, scan);
    return true;
}

void get_point(Point* point, int* column, FILE* scan)
{
    point->x = get_number(column, scan);
    unexpect(',', column, ER_UNEXPECT_COMMA, scan);

    point->y = get_number(column, scan);
}

void end_of_line(int* column, FILE* scan)
{
    char ch;
    while ((ch = getc(scan)) != '\n' && ch != EOF) {
        if (ch != ' ') {
            print_error(*column, ER_UNEXPECT_TOKEN);
            exit(EXIT_FAILURE);
        }
        *column += 1;
    }
}

void take_info_circle(Circle* circle, int* column, FILE* scan)
{
        get_point(&circle->point, column, scan);
        expect(',', column, ER_EXPECT_COMMA, scan);

        circle->radius = get_number(column, scan);

        expect(')', column, ER_BACK_BRACE, scan);

        end_of_line(column, scan);
    
    circle->perimeter = 2 * 3.14 * circle->radius;
    circle->area = 3.14 * circle->radius * circle->radius;
}

void show_info_circle(Circle* circle)
{
    printf("circle(%.2f %.2f, %.2f)\n",
           circle->point.x,
           circle->point.y,
           circle->radius);
    printf("area = %.4f\n", circle->area);
    printf("perimeter = %.4f\n", circle->perimeter);
}


int main()
{
    char geom[NAME_SIZE] = {0};
    char ch;
    int column;

    puts("Enter a geometric shape (or q for exit):");
    while ((ch = getc(stdin)) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, stdin);
                    printf("\nYou have entered: \n");
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