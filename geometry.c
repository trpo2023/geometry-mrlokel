#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct point {
    double x;
    double y;
} Point;

typedef struct circle {
    Point point;
    double radius;
    double perimetr;
    double area;
} Circle;

enum Errors {
    _FILE,
    NOT_FILE,
    ERROR_NAME,
    ERROR_NOT_DOUBLE,
    ERROR_BACK_BRACE,
    ERROR_UNEXPECT_TOKEN,
    ERROR_EXPECT_COMMA,
    ERROR_UNEXPECT_COMMA,
};

void print_error(int column, int status, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        char temp[256];
        fseek(file, -column - 1, SEEK_CUR);
        fgets(temp, 255, file);
        printf("\n%s", temp);
    }

    for (int i = 0; i < column; i++) {
        putchar(' ');
    }
    printf("\e[1;31m^\e[0m\n");
    switch (status) {
    case ERROR_NAME:
        printf("\e[1;31mError at column %d: expected 'circle'\e[0m\n", column);
        break;
    case ERROR_NOT_DOUBLE:
        printf("\e[1;31mError at column %d: expected '<double>'\e[0m\n", column);
        break;
    case ERROR_BACK_BRACE:
        printf("\e[1;31mError at column %d: expected ')'\e[0m\n", column);
        break;
    case ERROR_UNEXPECT_TOKEN:
        printf("\e[1;31mError at column %d: unexpected token\e[0m\n", column);
        break;
    case ERROR_EXPECT_COMMA:
        printf("\e[1;31mError at column %d: expected ','\e[0m\n", column);
        break;
    case ERROR_UNEXPECT_COMMA:
        printf("\e[1;31mError at column %d: unexpected ','\e[0m\n", column);
    }
}

void to_lower_string(char* string)
{
    while (*string != '\0') {
        *string = tolower(*string);
        string++;
    }
}

void del_space(int* column, FILE* file)
{
    char ch;
    while ((ch = getc(file)) == ' ') {
        *column += 1;
        continue;
    }
    if (ch != ' ')
        ungetc(ch, file);
}

double get_number(int* column, int is_file, FILE* file)
{
    char temp[25];
    char ch;
    int point_count = 0;
    int i = 0;
    int minus_count = 0;

    del_space(column, file);

    while ((ch = getc(file)) != ' ') {
        temp[i] = ch;

        if (temp[i] == '.') {
            point_count++;
            if (point_count > 1) {
                if (is_file == _FILE)
                    print_error(*column + i + 1, ERROR_NOT_DOUBLE, _FILE, file);
                else
                    print_error(*column + i + 1, ERROR_NOT_DOUBLE, NOT_FILE, file);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == '-') {
            minus_count++;
            if (minus_count > 1) {
                if (is_file == _FILE)
                    print_error(*column + i + 1, ERROR_NOT_DOUBLE, _FILE, file);
                else
                    print_error(*column + i + 1, ERROR_NOT_DOUBLE, NOT_FILE, file);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == ')') {
            ungetc(temp[i], file);
            i++;
            break;
        }

        if (temp[i] == ',') {
            ungetc(temp[i], file);
            break;
        }

        if (temp[i] == '(') {
            i++;
            if (is_file == _FILE)
                print_error(*column + i, ERROR_BACK_BRACE, _FILE, file);
            else
                print_error(*column + i, ERROR_BACK_BRACE, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        if (!isdigit(temp[i]) && temp[i] != '.' && temp[i] != '-') {
            i++;
            if (is_file == _FILE)
                print_error(*column + i, ERROR_NOT_DOUBLE, _FILE, file);
            else
                print_error(*column + i, ERROR_NOT_DOUBLE, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        i++;
    }
    del_space(column, file);
    *column += i + 1;
    char* eptr;
    return strtod(temp, &eptr);
}

bool expect(char expect, int* column, int status, int is_file, FILE* file)
{
    char ch;
    if ((ch = getc(file)) == expect) {
        return true;
    } else {
        if (is_file == _FILE)
            print_error(*column, status, _FILE, file);
        else
            print_error(*column, status, NOT_FILE, file);
        exit(EXIT_FAILURE);
    }
}

bool unexpect(char unexpect, int* column, int status, int is_file, FILE* file)
{
    char ch;
    if ((ch = getc(file)) == unexpect) {
        if (is_file == _FILE)
            print_error(*column, status, _FILE, file);
        else
            print_error(*column, status, NOT_FILE, file);
        exit(EXIT_FAILURE);
    }
    ungetc(ch, file);
    return true;
}

void get_point(Point* point, int* column, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        point->x = get_number(column, _FILE, file);
        unexpect(',', column, ERROR_UNEXPECT_COMMA, _FILE, file);

        point->y = get_number(column, _FILE, file);
    } else {
        point->x = get_number(column, NOT_FILE, file);
        unexpect(',', column, ERROR_UNEXPECT_COMMA, NOT_FILE, file);

        point->y = get_number(column, NOT_FILE, file);
    }
}

void end_of_line(int* column, int is_file, FILE* file)
{
    char ch;
    while ((ch = getc(file)) != '\n' && ch != EOF) {
        if (ch != ' ') {
            if (is_file == _FILE)
                print_error(*column, ERROR_UNEXPECT_TOKEN, _FILE, file);
            else
                print_error(*column, ERROR_UNEXPECT_TOKEN, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }
        *column += 1;
    }
}

void take_info_circle(Circle* circle, int* column, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        get_point(&circle->point, column, _FILE, file);
        expect(',', column, ERROR_EXPECT_COMMA, _FILE, file);

        circle->radius = get_number(column, _FILE, file);

        expect(')', column, ERROR_BACK_BRACE, _FILE, file);

        end_of_line(column, _FILE, file);
    } else {
        get_point(&circle->point, column, NOT_FILE, file);
        expect(',', column, ERROR_EXPECT_COMMA, NOT_FILE, file);

        circle->radius = get_number(column, NOT_FILE, file);

        expect(')', column, ERROR_BACK_BRACE, NOT_FILE, file);

        end_of_line(column, NOT_FILE, file);
    }
    circle->perimetr = 2 * 3.14 * circle->radius;
    circle->area = 3.14 * circle->radius * circle->radius;
}

void show_info_circle(Circle* circle)
{
    printf("circle(%.2f %.2f, %.2f)\n",
           circle->point.x,
           circle->point.y,
           circle->radius);
    printf("perimeter = %.4f\n", circle->perimetr);
    printf("area = %.4f\n", circle->area);
}

void parser_file(FILE* file)
{
    char geom[25] = {0};
    char ch;
    int column;

    while ((ch = getc(file)) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                to_lower_string(geom);
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, _FILE, file);
                    printf("\nYou have entered: \n");
                    show_info_circle(&circle);
                    break;
                } else {
                    print_error(0, ERROR_NAME, _FILE, file);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ERROR_BACK_BRACE, _FILE, file);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(file)) != '\n');
    }
}

int main(int arg, char* argv[])
{
    FILE* file = NULL;
    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("\e[1;31mError\e[0m: can't open file \e[1;35m\"%s\"\e[0m\n",
                argv[1]);
        exit(EXIT_FAILURE);
    } else {
        parser_file(file);
        fclose(file);
    }

    return 0;
}