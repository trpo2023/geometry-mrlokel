#include <ctest.h>
#include <libgeometry/func.h>
#include <stdio.h>

CTEST(func, del_space)
{
    FILE* file = fopen("read_str.txt", "w+");
    fprintf(file, "g g g");
    fseek(file, 0, SEEK_SET);

    int len = 10;
    int* count = &len;

    del_space(count, file);
}

CTEST(func, expect)
{
    FILE* file = fopen("read_str.txt", "w+");
    fprintf(file, "( g g");
    fseek(file, 0, SEEK_SET);

    int len = 10;
    int* count = &len;
    char expected_ch = '(';

    bool result = expect(expected_ch, count, ER_UNEXPECT_COMMA, file);
    bool expect = true;

    ASSERT_EQUAL(expect, result);
}

CTEST(func, unexpect)
{
    FILE* file = fopen("read_str.txt", "w+");
    fprintf(file, " g g");
    fseek(file, 0, SEEK_SET);

    int len = 10;
    int* count = &len;
    char unexpected_ch = ',';

    bool result = unexpect(unexpected_ch, count, ER_UNEXPECT_COMMA, file);
    bool expect = true;

    ASSERT_EQUAL(expect, result);
}

CTEST(func, get_point)
{
    FILE* file = fopen("read_str.txt", "w+");
    fprintf(file, " g g");
    fseek(file, 0, SEEK_SET);

    int len = 10;
    int* count = &len;

    Circle circle;
    get_point(&circle->point, count, file);
}