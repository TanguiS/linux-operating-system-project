/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

/**
 * @file test_unit_cameras.c
 * @brief /!\ criterion unit test, test the cameras functions /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "counter_intelligence_officer/cameras.h"
#include "criterion/criterion.h"
#include "memory/memory.h"
#include "utils/other.h"
#include "utils/path.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern bool isCharactersOutside( const cell_t* cell );

extern bool isSuspiciousHours( const double time );

extern bool isAnHourPassed( const double currentHour, const double currentTime );

extern bool isPointInMap( const point_t* point, const map_t* map );

extern bool isCharacterCloseToAnEnterprise( const point_t* location, const map_t* map );

volatile bool runnable;

volatile bool run;

int shm;
memory_t* memory;

void setup( void )
{
    shm = createMemory( SHM_NAME );
    memory = loadMemory( shm );
}

void teardown( void )
{
    cleanup_memory( memory );
    unloadMemory( memory );
    closeMemory( shm );
}

Test( is_point_in_map, in_map, .init = setup, .fini = teardown )
{
    int i, j;
    point_t point;
    for ( i = 0; i < MAP_SIZE; i++ )
    {
        for ( j = 0; j < MAP_SIZE; j++ )
        {
            point.row = i;
            point.column = j;
            cr_assert( isPointInMap( &point, &memory->map ) );
        }
    }
}

Test( is_point_in_map, not_in_map, .init = setup, .fini = teardown )
{
    int i, j;
    point_t point;
    i = -1;
    point.row = i;
    for ( j = 0; j < MAP_SIZE; j++ )
    {
        point.column = j;
        cr_assert_not( isPointInMap( &point, &memory->map ) );
    }
}

Test( is_suspicious_hours, it_is_suspicious_hours, .init = setup, .fini = teardown )
{
    double hours;
    for ( hours = 1.; hours < 7.; hours += 0.167 )
    {
        cr_assert( isSuspiciousHours( hours ) );
    }
}

Test( is_suspicious_hours, it_is_not_suspicious_hours, .init = setup, .fini = teardown )
{
    double hours;
    for ( hours = 7.; hours < 24.; hours += 0.167 )
    {
        cr_assert_not( isSuspiciousHours( hours ) );
    }
}

Test( is_an_hour_passed, yes_an_hour_passed, .init = setup, .fini = teardown )
{
    cr_assert( isAnHourPassed( 7., 8. ) );
}

Test( is_an_hour_passed, no_an_hour_passed, .init = setup, .fini = teardown )
{
    double begin = 7., i;
    for ( i = begin; i < 8.; i += 0.167 )
    {
        cr_assert_not( isAnHourPassed( 7., i ) );
    }
}

Test( is_character_close_to_an_enterprise, no, .init = setup, .fini = teardown )
{
    point_t current;
    int i, j;
    for ( i = 0; i < MAP_SIZE; i++ )
    {
        for ( j = 0; j < MAP_SIZE; j++ )
        {
            if ( memory->map.cells[i][j].type == RESIDENTIAL_BUILDING || memory->map.cells[i][j].type == RESIDENTIAL_BUILDING )
            {
                current.row = i;
                current.column = j;
                cr_assert_not( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
            }
        }
    }
    current.row = 3;
    current.column = 0;
    cr_assert_not( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 3;
    current.column = 6;
    cr_assert_not( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
}

Test( is_character_close_to_an_enterprise, yes, .init = setup, .fini = teardown )
{
    point_t current;
    current.row = 0;
    current.column = 0;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 0;
    current.column = 2;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 0;
    current.column = 4;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 0;
    current.column = 5;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 1;
    current.column = 1;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 1;
    current.column = 3;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 1;
    current.column = 4;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 1;
    current.column = 6;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 5;
    current.column = 0;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
    current.row = 5;
    current.column = 3;
    cr_assert( isCharacterCloseToAnEnterprise( &current, &memory->map ) );
}