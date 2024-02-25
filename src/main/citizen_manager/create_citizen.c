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
 * @file action_citizen.c
 * @brief /!\ source, manage the creation of all the citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "citizen_manager/create_citizen.h"
#include "citizen_manager/action_citizen.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <unistd.h>

extern thread_storage_t citizens; /**<-- the datas to handle the threads */
extern memory_t* memory;          /**<-- the shared memory */

/**
 * @brief Initialize all the thread
 * 
 */
static void initCitizensThreadStorage();

/**
 * @brief Create all the Citizens Thread object
 * 
 * @param pargs 
 * @param tid 
 */
static void createCitizensThread( citizen_pargs_t* pargs, pthread_t* tid );

static void initCitizensThreadStorage()
{
    int i;
    for ( i = 0; i < NB_OF_CITIZENS; i++ )
    {
        if ( pthread_mutex_init( &citizens.mutex[i], NULL ) != 0 )
        {
            handleFatalError( "mutex init error\n" );
        }
    }
    if ( pthread_mutex_init( &citizens.accessMemory, NULL ) != 0 )
    {
        handleFatalError( "Mutex init error\n" );
    }
    if ( pthread_cond_init( &citizens.onTurnAction, NULL ) != 0 )
    {
        handleFatalError( "Cond init error\n" );
    }
}

static void createCitizensThread( citizen_pargs_t* pargs, pthread_t* tid )
{
    int i;
    for ( i = 0; i < NB_OF_CITIZENS; i++ )
    {
        tid[i] = i;
        pargs[i].idCitizen = i;
        pargs[i].goToMarket = false;
        pargs[i].flipCoin = true;
        pargs[i].marketHours = 0.;
        pargs[i].updateState = memory->characters.citizens_state;
        if ( pthread_create( &tid[i], NULL, handleCitizensThreadCall, &pargs[i] ) != 0 )
        {
            handleFatalError( "Thread creation error\n" );
        }
    }
}

void createCitizensThreads( pthread_t* tid, citizen_pargs_t* pargs )
{
    srand48_r( time( NULL ), &citizens.randBuffer );
    initCitizensThreadStorage();
    createCitizensThread( pargs, tid );
}

void deleteCitizensThreadStorage()
{
    int i;
    for ( i = 0; i < NB_OF_CITIZENS; i++ )
    {
        if ( pthread_mutex_destroy( &citizens.mutex[i] ) != 0 )
        {
            handleFatalError( "Mutex destroy error\n" );
        }
    }
    if ( pthread_mutex_destroy( &citizens.accessMemory ) != 0 )
    {
        handleFatalError( "Mutex destroy error\n" );
    }
    if ( pthread_cond_destroy( &citizens.onTurnAction ) != 0 )
    {
        handleFatalError( "Cond destroy error\n" );
    }
}