#pragma once
#include <stdbool.h>
#include <stdio.h>

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

void print_error(int column, int status);
void del_space(int* column, FILE* scan);
double get_number(int* column, FILE* scan);
bool expect(char expect, int* column, int status, FILE* scan);
bool unexpect(char unexpect, int* column, int status, FILE* scan);
void get_point(Point* point, int* column, FILE* scan);
void end_of_line(int* column, FILE* scan);
void set_info_circle(Circle* circle, int* column, FILE* scan);
void show_info_circle(Circle* circle);
