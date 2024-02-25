#include "utils/other.h"
#include "utils/path.h"
#include <criterion/criterion.h>
#include <stdio.h>

Test( test_possible_points_normal, sample )
{
    point_t start;
    point_t next[2];
    start.row = 1;
    start.column = 1;
    int drow = 0;
    int dcol = -1;
    getPossiblePoints( start, drow, dcol, next );
    cr_assert_eq( next[0].column, 0 );
    cr_assert_eq( next[0].row, 0 );
    cr_assert_eq( next[1].column, 0 );
    cr_assert_eq( next[1].row, 2 );
    fflush( stdout );
}

Test( test_possible_points_border_row, sample )
{
    point_t start;
    point_t next[2];
    start.row = 0;
    start.column = 0;
    int drow = 1;
    int dcol = 0;
    getPossiblePoints( start, drow, dcol, next );
    cr_assert_eq( next[0].column, ERROR );
    cr_assert_eq( next[0].row, 1 );
    cr_assert_eq( next[1].column, 1 );
    cr_assert_eq( next[1].row, 1 );
    fflush( stdout );
}

Test( test_possible_points_border_col, sample )
{
    point_t start;
    point_t next[2];
    start.row = 0;
    start.column = 0;
    int drow = 0;
    int dcol = 1;
    getPossiblePoints( start, drow, dcol, next );
    cr_assert_eq( next[0].column, 1 );
    cr_assert_eq( next[0].row, ERROR );
    cr_assert_eq( next[1].column, 1 );
    cr_assert_eq( next[1].row, 1 );
    fflush( stdout );
}

Test( another_test, sample )
{
    point_t start;
    point_t next[2];
    start.row = 6;
    start.column = 4;
    int drow = -1;
    int dcol = -1;
    getPossiblePoints( start, drow, dcol, next );
    cr_assert_eq( next[0].column, 4 );
    cr_assert_eq( next[0].row, 5 );
    cr_assert_eq( next[1].column, 3 );
    cr_assert_eq( next[1].row, 6 );
    fflush( stdout );
}

Test( another_test2, sample )
{
    point_t start;
    point_t next[2];
    start.row = 3;
    start.column = 6;
    int drow = 1;
    int dcol = 0;
    getPossiblePoints( start, drow, dcol, next );
    // printf( "RESULT = %d, %d ; %d, %d", next[0].row, next[0].column, next[1].row, next[1].column );
    cr_assert_eq( next[0].column, 5 );
    cr_assert_eq( next[0].row, 4 );
    cr_assert_eq( next[1].column, -1 );
    cr_assert_eq( next[1].row, 4 );
    fflush( stdout );
}
