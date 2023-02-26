#include <stdio.h>

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