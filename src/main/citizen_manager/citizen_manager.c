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
 * @file citizen_manager.h
 * @brief /!\ header, manage the citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "citizen_manager/citizen_manager.h"
#include "citizen_manager/action_citizen.h"
#include "citizen_manager/create_citizen.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

thread_storage_t citizens;     /**<-- the datas to handle the threads */
memory_t* memory;              /**<-- the shared memory */
volatile bool runnable = true; /**<-- Status of the simulation */

/**
 * @brief Properly delete all the threads
 * 
 * @param shm 
 */
static void deleteCitizenManager( int shm )
{
    unloadMemory( memory );
    closeMemory( shm );
    deleteCitizensThreadStorage();
}

void runCitizenManager()
{
    int i;
    int shm;
    pthread_t tid[NB_OF_CITIZENS];
    citizen_pargs_t pargs[NB_OF_CITIZENS];
    shm = openMemory( SHM_NAME );
    memory = loadMemory( shm );

    createCitizensThreads( tid, pargs );

    while ( runnable )
    {
        pause();
        if ( ENABLE_LOG_CITIZENS )
        {
            log_green( "number people before wake up, home : %d, work : %d, moving : %d", memory->characters.citizens_state[INDEX_HOME], memory->characters.citizens_state[INDEX_WORK], memory->characters.citizens_state[INDEX_MOVING] );
        }
        pthread_cond_broadcast( &citizens.onTurnAction );
    }
    pthread_cond_broadcast( &citizens.onTurnAction );

    for ( i = 0; i < NB_OF_CITIZENS; i++ )
    {
        int retval = pthread_join( tid[i], NULL );
        if ( retval != 0 )
        {
            handleFatalError( "Thread join error\n" );
        }
    }
    deleteCitizenManager( shm );
}
