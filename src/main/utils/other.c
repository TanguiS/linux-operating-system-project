#include "utils/other.h"
#include "memory/memory.h"
#include <limits.h>
#include <stdio.h>

bool compare_cells( cell_t* cell1, cell_t* cell2 )
{
    return cell1->coords.column == cell2->coords.column && cell1->coords.row == cell2->coords.row;
}

bool compare_points( point_t firstPoint, point_t secondPoint )
{
    return firstPoint.column == secondPoint.column && firstPoint.row == secondPoint.row;
}

static int manhattanDistancePoint( point_t firstPoint, point_t secondPoint )
{
    return abs( firstPoint.column - secondPoint.column ) + abs( firstPoint.row - secondPoint.row );
}

point_t getClosestSuperMarket( point_t start, cell_t map[MAP_SIZE][MAP_SIZE] )
{
    int i, j, currentDistance;
    int minDistance = INT_MAX;
    point_t currentSuperMarket;
    currentSuperMarket.row = ERROR;
    currentSuperMarket.column = ERROR;
    for ( i = 0; i < MAP_SIZE; i++ )
    {
        for ( j = 0; j < MAP_SIZE; j++ )
        {
            currentDistance = manhattanDistancePoint( start, map[i][j].coords );
            if ( map[i][j].type == SUPERMARKET && map[i][j].currentPop < SUPERMARKET_POP && minDistance > currentDistance )
            {
                minDistance = currentDistance;
                currentSuperMarket.row = i;
                currentSuperMarket.column = j;
            }
        }
    }
    return currentSuperMarket;
}

cell_t* getCellFromPoint( point_t point, cell_t map[MAP_SIZE][MAP_SIZE] )
{
    return &map[point.row][point.column];
}

double getRandomTimeBetween( double start, double end, struct drand48_data* randBuffer )
{
    long int rand;
    if ( end < LIMIT )
    {
        end = 24.;
    }
    lrand48_r( randBuffer, &rand );
    return start + (double)( rand % (int)( ( end - start ) * NUMBER_TURNS_IN_HOUR ) ) / TIME_TURN;
}

void* getCharacterFromId( int id, int* typeCharacter, characters_t* characters )
{
    if ( id < 0 ) {
        *typeCharacter = ERROR;
        return NULL;
    }
    if ( id < NB_OF_SPIES )
    {
        /* it is a spy */
        *typeCharacter = SPY_ID;
        return (void*)( &( characters->spies[id] ) );
    }
    if ( id < NB_OF_SPIES + NB_CASE_OFFICER )
    {
        /* it is the case officer */
        *typeCharacter = CASE_OF_ID;
        return (void*)&characters->case_officers[id - NB_OF_SPIES];
    }
    if ( id < NB_OF_SPIES + NB_CASE_OFFICER + NB_OF_COUNTER )
    {
        /* it is the counter intelligence officer */
        *typeCharacter = COUNTER_ID;
        return (void*)&characters->counter_officers[id - NB_OF_SPIES - NB_OF_COUNTER];
    }
    /* it is a citizen */
    *typeCharacter = CITIZEN_ID;
    return (void*)&characters->citizens[id - NB_OF_SPIES - NB_CASE_OFFICER - NB_OF_COUNTER];
}

point_t* getPointFromId( int id, characters_t* characters )
{
    static point_t err = {-1, -1};
    int type;
    void* buff = getCharacterFromId( id, &type, characters );
    citizen_t* buffCitizen;
    spy_t* buffSpy;
    case_officer_t* buffCaseOfficer;
    counterintelligence_officer_t* buffCIO;
    if ( type == ERROR ) {
        return &err;
    }
    if ( type == SPY_ID )
    {
        buffSpy = (spy_t*)buff;
        return &buffSpy->location;
    }
    if ( type == CASE_OF_ID )
    {
        buffCaseOfficer = (case_officer_t*)buff;
        return &buffCaseOfficer->location;
    }
    if ( type == COUNTER_ID )
    {
        buffCIO = (counterintelligence_officer_t*)buff;
        return &buffCIO->location;
    }
    buffCitizen = (citizen_t*)buff;
    return &buffCitizen->location;
}

cell_t* getCellFromId( int id, characters_t* characters, cell_t map[MAP_SIZE][MAP_SIZE] )
{
    point_t* point = getPointFromId( id, characters );
    return getCellFromPoint( *point, map );
}