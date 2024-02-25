#include "utils/path.h"
#include "common/logger.h"
#include "memory/memory.h"
#include "utils/other.h"
#include <limits.h>
#include <stdio.h>

typedef struct node
{
    point_t value;
    struct node* next;
} node_t;

static node_t* createNode( point_t value )
{
    node_t* node = malloc( sizeof( node_t ) );
    node->value = value;
    node->next = NULL;
    return node;
}

static void deleteList( node_t* node )
{
    node_t* current = node;
    node_t* toDelete;
    while ( current != NULL )
    {
        toDelete = current;
        current = current->next;
        free( toDelete );
    }
}

static node_t* pushValue( node_t* list, point_t value )
{
    node_t* start = list;
    if ( list == NULL )
    {
        return createNode( value );
    }
    while ( list->next != NULL )
    {
        list = list->next;
    }
    list->next = createNode( value );
    return start;
}
static bool canAccess( cell_t* cell )
{
    return cell->currentPop < cell->maxPop && cell->type != RESIDENTIAL_BUILDING && cell->type != COMPANY;
}

static bool canAccessOrDestination( cell_t* cell, cell_t* destination )
{
    if ( compare_cells( cell, destination ) )
    {
        return true;
    }
    return canAccess( cell );
}

point_t getNextPoint( point_t start, point_t end, cell_t map[MAP_SIZE][MAP_SIZE] )
{
    int i, j;
    int newRow, newCol;
    node_t* list = createNode( start );
    node_t* toDelete;
    int explored[MAP_SIZE][MAP_SIZE];
    point_t previous[MAP_SIZE][MAP_SIZE];
    for ( i = 0; i < MAP_SIZE; i++ )
    {
        for ( j = 0; j < MAP_SIZE; j++ )
        {
            explored[i][j] = 0;
        }
    }
    explored[start.row][start.column] = 1;
    point_t current, previousCell, toPush;
    while ( 1 )
    {
        current = list->value;
        toDelete = list;
        list = list->next;
        free( toDelete );
        if ( compare_points( current, end ) )
        {
            break;
        }
        for ( int i = -1; i < 2; i++ )
        {
            for ( int j = -1; j < 2; j++ )
            {
                newRow = current.row + i;
                newCol = current.column + j;
                if ( newRow < MAP_SIZE && newRow >= 0 && newCol < MAP_SIZE && newCol >= 0 && ( i != 0 || j != 0 ) )
                {
                    if ( canAccessOrDestination( &map[newRow][newCol], &map[end.row][end.column] ) && !explored[newRow][newCol] )
                    {
                        toPush.column = newCol;
                        toPush.row = newRow;
                        explored[newRow][newCol] = 1;
                        previous[newRow][newCol] = current;
                        list = pushValue( list, toPush );
                    }
                }
            }
        }
    }
    current = previous[end.row][end.column];
    previousCell = end;
    while ( !compare_points( current, start ) )
    {
        previousCell = current;
        current = previous[current.row][current.column];
    }
    deleteList( list );
    return previousCell;
}

static bool hasError( point_t point )
{
    return point.row == ERROR || point.column == ERROR;
}

static point_t chooseBetweenPoints( point_t point1, point_t point2, point_t optimized, cell_t map[MAP_SIZE][MAP_SIZE], struct drand48_data* randBuffer )
{
    if ( hasError( point1 ) && !hasError( point2 ) )
    {
        if ( canAccess( &map[point2.row][point2.column] ) )
        {
            return point2;
        }
        return optimized;
    }
    if ( !hasError( point1 ) && hasError( point2 ) )
    {
        if ( canAccess( &map[point1.row][point1.column] ) )
        {
            return point1;
        }
        return optimized;
    }
    bool canAcces1 = canAccess( &map[point1.row][point1.column] );
    bool canAcces2 = canAccess( &map[point2.row][point2.column] );

    if ( !canAcces1 && !canAcces2 )
    {
        return optimized;
    }
    if ( canAcces1 && !canAcces2 )
    {
        return point1;
    }
    if ( !canAcces1 && canAcces2 )
    {
        return point2;
    }
    double rand;
    drand48_r( randBuffer, &rand );
    if ( rand > 0.5 )
    {
        return point1;
    }
    return point2;
}

void getPossiblePoints( point_t start, int drow, int dcol, point_t points[2] )
{
    if ( drow == 0 )
    {
        points[0].column = start.column + dcol;
        points[1].column = start.column + dcol;
        if ( start.row - 1 >= 0 )
        {
            points[0].row = start.row - 1;
        }
        else
        {
            points[0].row = ERROR;
        }
        if ( start.row + 1 < MAP_SIZE )
        {
            points[1].row = start.row + 1;
        }
        else
        {
            points[1].row = ERROR;
        }
        return;
    }
    if ( dcol == 0 )
    {
        points[0].row = start.row + drow;
        points[1].row = start.row + drow;
        if ( start.column - 1 >= 0 )
        {
            points[0].column = start.column - 1;
        }
        else
        {
            points[0].column = ERROR;
        }
        if ( start.column + 1 < MAP_SIZE )
        {
            points[1].column = start.column + 1;
        }
        else
        {
            points[1].column = ERROR;
        }
        return;
    }
    points[0].row = start.row + drow;
    points[0].column = start.column;
    points[1].row = start.row;
    points[1].column = start.column + dcol;
}

bool isValidePoints( point_t point )
{
    return point.row != ERROR && point.column != ERROR;
}

point_t getRandomNextPointFromDrandGenerator( point_t start, point_t end, cell_t map[MAP_SIZE][MAP_SIZE], struct drand48_data* randBuffer )
{
    point_t optimizedPoint = getNextPoint( start, end, map );
    point_t otherPoints[2];
    // 80% to go to the good point
    double randRightDest;
    drand48_r( randBuffer, &randRightDest );
    if ( randRightDest >= 0.8 )
    {
        return optimizedPoint;
    }
    getPossiblePoints( start, optimizedPoint.row - start.row, optimizedPoint.column - start.column, otherPoints );
    point_t choice = chooseBetweenPoints( otherPoints[0], otherPoints[1], optimizedPoint, map, randBuffer );
    return choice;
}

void updatePopulation( point_t currentLocation, point_t nextLocation, cell_t map[MAP_SIZE][MAP_SIZE] )
{
    map[currentLocation.row][currentLocation.column].currentPop--;
    map[nextLocation.row][nextLocation.column].currentPop++;
}