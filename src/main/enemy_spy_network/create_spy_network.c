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
 * @file create_spy_network.c
 * @brief /!\ function which creat spy threads /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "./enemy_spy_network/create_spy_network.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <unistd.h>

extern thread_storage_spies spies;

/**
 * @brief initialize threads which reprensent spies
*/
static void initSpiesThreadStorage() {
    int i;
    for ( i = 0; i < NB_OF_SPIES; i++ )
    {
        if ( pthread_mutex_init( &spies.mutex[i], NULL ) != 0 )
        {
            handleFatalError( "mutex init error\n" );
        }
    }
    if ( pthread_mutex_init( &spies.accessMemory, NULL ) != 0 )
    {
        handleFatalError( "Mutex init error\n" );
    }
    if ( pthread_cond_init( &spies.onTurnAction, NULL ) != 0 )
    {
        handleFatalError( "Cond init error\n" );
    }
}


/**
 * @brief create thread which represents a spy
 * 
 * @param tid
 * @param pargs
*/
static void createSpiesThread( spies_pargs * pargs, pthread_t* tid ) {
    int i, j;
    for ( i = 0; i < NB_OF_SPIES; i++ )
    {
        tid[i] = i;
        pargs[i].night_objective = GO_BACK_HOME;
        pargs[i].id_spy = i;
        pargs[i].observation_turn_number = -1;
        pargs[i].stealing_turn_number = 0;
        pargs[i].can_i_steal = false;
        pargs[i].has_time_to_steal = false;
        pargs[i].has_a_message = false;
        pargs[i].stealing_turn_number = 0;
        pargs[i].flag_to_steal_next_night = false;
        pargs[i].want_to_steal = false;
        pargs[i].turn_number_to_move = 1;
        pargs[i].turn_number_to_heal = 0;
        pargs[i].night_objective = GO_BACK_HOME;
        pargs[i].day_objective = HOME;
        for ( j = 0; j < NB_OF_COMPANY; j++ )
        {
            pargs[i].list_of_companies_visited[j] = false;
        }
        if ( pthread_create( &tid[i], NULL, handleSpiesThreadCall, &pargs[i] ) != 0 )
        {
            handleFatalError( "Thread creation error\n" );
        }
    }
}

void createSpiesThreads( pthread_t* tid, spies_pargs* pargs )
{
    srand48_r( time( NULL ), &spies.randBuffer );
    initSpiesThreadStorage();
    createSpiesThread( pargs, tid );
}

void deleteSpiesThreadStorage()
{
    int i;
    for ( i = 0; i < NB_OF_SPIES; i++ )
    {
        if ( pthread_mutex_destroy( &spies.mutex[i] ) != 0 )
        {
            handleFatalError( "Mutex destroy error\n" );
        }
    }
    if ( pthread_mutex_destroy( &spies.accessMemory ) != 0 )
    {
        handleFatalError( "Mutex destroy error\n" );
    }
    if ( pthread_cond_destroy( &spies.onTurnAction ) != 0 )
    {
        handleFatalError( "Cond destroy error\n" );
    }
}