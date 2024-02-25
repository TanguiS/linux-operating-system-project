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
 * @file action_spy_network.c
 * @brief /!\ functions which manage global spy actions /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "./enemy_spy_network/action_spy_network.h"
#include "./enemy_spy_network/day_action_spies.h"
#include "./enemy_spy_network/fight_actions.h"
#include "./enemy_spy_network/night_action_spies.h"
#include <math.h>
#include <stdbool.h>

extern memory_t* memory;
extern volatile bool runnable;
spy_t* spy;

/**
 * @brief Manage a spy considering the time 
 * 
 * @param spy_args
*/
static void manageTurnAction( spies_pargs* spy_arg ) {
    double timer = memory->simulationTime;
    spy = &memory->characters.spies[spy_arg->id_spy];
    if ( spy->health_points_has_changed == true ) {
        updateTurnMove( spy_arg );
        spy->health_points_has_changed = false;
    }
    if ( ( timer >= DAY_BEGIN && timer < DAY_END ) ) {
        chooseDayAction( timer, spy_arg->id_spy, spy_arg );
        return;
    }
    else if ( timer >= DAY_END || timer < DAY_BEGIN )
    {
        chooseNightAction( timer, spy_arg );
    }
}

void* handleSpiesThreadCall( void* args )
{

    int id = ( (spies_pargs*)args )->id_spy;

    while ( runnable )
    {
        pthread_cond_wait( &spies.onTurnAction, &spies.mutex[id] );
        if ( ENABLE_LOG_SPY )
        {
            log_green( "RECEIVED SIGNAL" );
        }
        if ( !runnable )
        {
            return NULL;
        }
        if ( ENABLE_LOG_SPY )
        {
            pthread_mutex_lock( &spies.accessMemory );
            if ( id == 1 )
            {
                log_debug( "\nHi I am a thread and i have the id spy : %d, at the time %f", id, memory->simulationTime );
            }
            fflush( stdout );
            pthread_mutex_unlock( &spies.accessMemory );
        }
        pthread_mutex_lock( &spies.accessMemory );
        manageTurnAction( (spies_pargs*)args );
        pthread_mutex_unlock( &spies.accessMemory );
    }
    return NULL;
}

void manageHome( spies_pargs* args )
{
    if ( compare_points( spy->location, spy->home ) )
    {
        if ( args->id_spy == 1 && ENABLE_LOG_SPY )
        {
            log_debug( "I am in my house : %d\n", args->id_spy );
        }
        if ( args->turn_number_to_heal == 0 && spy->health_points != 10 )
        {
            spy->health_points++;
            updateTurnMove( args );
        }
        else if ( args->turn_number_to_heal != 0 )
        {
            args->turn_number_to_heal--;
        }
    }
    return;
    manageMoving( args, spy->home );
}

double makeRandom()
{
    double rand;
    drand48_r( &spies.randBuffer, &rand );
    return rand;
}

/**
 * @brief Inform if a cell is a street
 * 
 * @param cell
*/
static bool canAccess( cell_t* cell ) {
    return cell->currentPop < cell->maxPop && cell->type != RESIDENTIAL_BUILDING && cell->type != COMPANY && cell->type != CITY_HALL;
}

void getCloseTo( spies_pargs* args )
{
    double rand;
    int position;
    int i = 0;
    int j = 0;
    int row;
    int column;
    int possibilities[][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
    rand = makeRandom();
    position = (int)( rand * 100 ) % 5 + 1;
    while ( i < position )
    {
        row = args->company_to_steal.cell.row + possibilities[j][0];
        column = args->company_to_steal.cell.column + possibilities[j][1];
        if ( column > MAP_SIZE || column < 0 || row > MAP_SIZE || row < 0 )
        {
            j = ( j + 1 ) % 4;
        }
        else
        {
            if ( canAccess( &( memory->map.cells[row][column] ) ) )
            {
                i++;
            }
            j = ( j + 1 ) % 4;
        }
    }
    if ( args->id_spy == 1 && ENABLE_LOG_SPY )
        log_debug( " %d I don't understand again %d %d ", position, row, column );

    args->point_closed_company.column = column;
    args->point_closed_company.row = row;
}

void updateTurnMove( spies_pargs* args )
{
    switch ( spy->health_points )
    {
    case 10:
        args->turn_number_to_move = 1;
        args->turn_number_to_heal = 0;
        break;
    case 9:
        args->turn_number_to_move = 1;
        args->turn_number_to_heal = 6;
        break;
    case 8:
        args->turn_number_to_move = 1;
        args->turn_number_to_heal = 12;
        break;
    case 7:
        args->turn_number_to_move = 2;
        args->turn_number_to_heal = 18;
        break;
    case 6:
        args->turn_number_to_move = 4;
        args->turn_number_to_heal = 24;
        break;
    case 5:
        args->turn_number_to_move = 8;
        args->turn_number_to_heal = 36;
        break;
    case 4:
        args->turn_number_to_move = 9;
        args->turn_number_to_heal = 48;
        break;
    case 3:
        args->turn_number_to_move = 10;
        args->turn_number_to_heal = 92;
        break;
    case 2:
        args->turn_number_to_move = 11;
        args->turn_number_to_heal = 128;
        break;
    case 1:
        args->turn_number_to_move = 20;
        args->turn_number_to_heal = 196;
        break;
    case 0:
        args->turn_number_to_move = RAND_MAX;
        args->turn_number_to_heal = RAND_MAX;
        break;
    default:
        break;
    }
}

void manageMoving( spies_pargs* args, point_t final )
{
    point_t destination;
    if ( ENABLE_LOG_SPY )
    {
        log_red( "the turn number  %d", args->turn_number_to_move );
    }
    if ( args->turn_number_to_move == 1 )
    {
        destination = getNextPoint( spy->location, final, memory->map.cells );
        updatePopulation( spy->location, destination, memory->map.cells );
        spy->location = destination;
        return;
    }
    args->turn_number_to_move--;
}