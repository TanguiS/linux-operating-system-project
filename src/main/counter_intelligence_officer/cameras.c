/**
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

/**
 * @file cameras.c
 * @brief /!\ source, manage the different cameras' action to inform the counter intelligence officer the suspicious citizen /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "counter_intelligence_officer/cameras.h"
#include "common/logger.h"
#include "memory/memory.h"
#include "utils/other.h"
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#define NUMBER_PEOPLE NB_OF_SPIES + NB_OF_CITIZENS + NB_CASE_OFFICER + NB_OF_COUNTER
#define SUSPICIOUS_NIGHT_BEGIN 1.
#define SUSPICIOUS_NIGHT_END 7.
#define INPUT 1
#define OUTPUT 0

static bool runnable; /*<-- Status of the Cameras */

typedef struct sigaction sigaction_t;

/**
 * @brief Args for Cameras
 *
 */
typedef struct cameras
{
    int currentHour;
    int shm;
    bool* wasAroundCompanyLastHour;
    int* suspiciousPipeId;
    characters_t* characters;
    map_t* map;
    memory_t* memory;
} cameras_t;

/**
 * @brief Check if there are Characters outside at the given Cell
 *
 * @param cell
 * @return true
 * @return false
 */
bool isCharactersOutside( const cell_t* cell )
{
    return cell->currentPop > 0 && cell->type != RESIDENTIAL_BUILDING;
}

/**
 * @brief Check if the given Time is at suspicious Hours (Night)
 *
 * @param time
 * @return true
 * @return false
 */
bool isSuspiciousHours( const double time )
{
    return time >= SUSPICIOUS_NIGHT_BEGIN && time < SUSPICIOUS_NIGHT_END;
}

/**
 * @brief Check if an Hour passed since the last check at the time currentTime
 *
 * @param currentHour
 * @param currentTime
 * @return true
 * @return false
 */
bool isAnHourPassed( const double currentHour, const double currentTime )
{
    return (int)currentHour + 1 == (int)currentTime;
}

/**
 * @brief Check if the given Point is in the Map
 *
 * @param point
 * @param map
 * @return true
 * @return false
 */
bool isPointInMap( const point_t* point, const map_t* map )
{
    return ( point->row >= 0 && point->row < map->rows ) && ( point->column >= 0 && point->column < map->columns );
}

/**
 * @brief Check if the given ID is the counter's one
 *
 * @param id
 * @param characters
 * @return true
 * @return false
 */
bool isCounterOfficer( int id, characters_t* characters )
{
    int type;
    getCharacterFromId( id, &type, characters );
    return type == COUNTER_ID;
}

/**
 * @brief Check if the given Location is around a Company but not ON a Company
 *
 * @param location
 * @param map
 * @return true
 * @return false
 */
bool isCharacterCloseToAnEnterprise( const point_t* location, const map_t* map )
{
    int i, j;
    point_t current;
    if ( map->cells[location->row][location->column].type == RESIDENTIAL_BUILDING )
    {
        return false;
    }
    if ( map->cells[location->row][location->column].type == SUPERMARKET )
    {
        return false;
    }
    if ( map->cells[location->row][location->column].type == CITY_HALL )
    {
        return false;
    }
    if ( map->cells[location->row][location->column].type == COMPANY )
    {
        return false;
    }
    for ( i = -1; i <= 1; i++ )
    {
        for ( j = -1; j <= 1; j++ )
        {
            current.row = location->row + i;
            current.column = location->column + j;
            if ( isPointInMap( &current, map ) && map->cells[current.row][current.column].type == COMPANY )
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Set the Signals object
 *
 */
void setSignals()
{
    sigaction_t startTurn;
    sigaction_t killCameras;

    memset( &startTurn, '\0', sizeof( startTurn ) );
    startTurn.sa_handler = &signalCamerasHandler;
    sigaction( SIGUSR1, &startTurn, NULL );

    memset( &killCameras, '\0', sizeof( killCameras ) );
    killCameras.sa_handler = &signalCamerasHandler;
    sigaction( SIGUSR2, &killCameras, NULL );
}

/**
 * @brief Kills the Cameras
 *
 * @param cameras
 */
void killCameras( cameras_t* cameras )
{
    free( cameras->wasAroundCompanyLastHour );
    close( *cameras->suspiciousPipeId );
    unloadMemory( cameras->memory );
    closeMemory( cameras->shm );
}

/**
 * @brief Execute the checking
 *
 * @param cameras
 */
void checkingSuspiciousCitizen( cameras_t* cameras )
{
    int suspiciousId;
    int i;
    for ( i = NUMBER_PEOPLE - 1; i >= 0; i-- )
    {
        if ( isCounterOfficer( i, cameras->characters ) )
        {
            continue;
        }
        if ( isSuspiciousHours( cameras->memory->simulationTime ) && isCharactersOutside( getCellFromId( i, cameras->characters, cameras->map->cells ) ) )
        {
            suspiciousId = i;
            write( *cameras->suspiciousPipeId, &suspiciousId, sizeof( suspiciousId ) );
            continue;
        }
        if ( isAnHourPassed( cameras->currentHour, cameras->memory->simulationTime ) )
        {
            if ( isCharacterCloseToAnEnterprise( getPointFromId( i, cameras->characters ), cameras->map ) )
            {
                if ( cameras->wasAroundCompanyLastHour[i] )
                {
                    suspiciousId = i;
                    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER ) {
                        log_bright_magenta( "  -Cameras : around company, aka louche boy : %d", suspiciousId );
                    }
                    write( *cameras->suspiciousPipeId, &suspiciousId, sizeof( suspiciousId ) );
                    continue;
                }
                cameras->wasAroundCompanyLastHour[i] = true;
            }
            else
            {
                cameras->wasAroundCompanyLastHour[i] = false;
            }
            if ( i == NUMBER_PEOPLE - 1 )
            {
                ++cameras->currentHour;
            }
        }
    }
}

void signalCamerasHandler( int signal )
{
    if ( signal == SIGUSR1 )
    {
        /* Cameras are checking */
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER ) {
            log_bright_magenta( "Cameras are running..." );
        }
    }
    if ( signal == SIGUSR2 )
    {
        runnable = false;
    }
}

void runCameras( int suspiciousPipeId[] )
{
    cameras_t cameras;
    cameras.suspiciousPipeId = &suspiciousPipeId[INPUT];
    cameras.shm = openMemory( SHM_NAME );
    cameras.memory = loadMemory( cameras.shm );
    cameras.currentHour = cameras.memory->simulationTime;
    cameras.map = &cameras.memory->map;
    cameras.characters = &cameras.memory->characters;
    cameras.wasAroundCompanyLastHour = (bool*)calloc( NUMBER_PEOPLE, sizeof( bool ) );

    setSignals();
    runnable = true;

    close( suspiciousPipeId[OUTPUT] );

    while ( true )
    {
        pause();
        if ( !runnable )
        {
            killCameras( &cameras );
            exit( EXIT_SUCCESS );
        }
        checkingSuspiciousCitizen( &cameras );
    }
}