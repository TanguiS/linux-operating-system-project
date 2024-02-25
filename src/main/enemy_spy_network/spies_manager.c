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
 * @file spies_manager.c
 * @brief /!\ function which manage spies in every turn /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "./enemy_spy_network/spies_manager.h"
#include "./case_officer/case_officer_manager.h"
#include "./case_officer/create_case_officer.h"
#include "./enemy_spy_network/action_spy_network.h"
#include "./enemy_spy_network/create_spy_network.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct sigaction sigaction_t;
thread_storage_spies spies;
extern thread_storage_t case_officer_thread;
memory_t* memory;
volatile bool runnable = 1;
sig_atomic_t beginFight = false;
sig_atomic_t is_double_domage;

/**
 * @brief Actions to delete properly spies
 * 
 * @param shm 
*/
static void deleteSpiesManager( int shm ) {
    unloadMemory( memory );
    closeMemory( shm );
    deleteSpiesThreadStorage();
}

void runSpiesManager()
{
    int i;
    int shm;
    pthread_t tid[NB_OF_SPIES];
    pthread_t tidCaseOfficer;
    spies_pargs pargs[NB_OF_SPIES];
    case_officer_pargs_t pargsCaseOfficer;
    shm = openMemory( SHM_NAME );
    memory = loadMemory( shm );

    memory->characters.counter_officers->targetPid = getpid();

    createCaseOfficerThread( &tidCaseOfficer, &pargsCaseOfficer );
    createSpiesThreads( tid, pargs );

    while ( runnable )
    {

        if ( ENABLE_LOG_SPY )
        {
            log_info( "Waiting signal at process : %d....", getpid() );
        }

        pause();
        if ( beginFight == true )
        {
            beginFight = false;
            continue;
        }
        if ( ENABLE_LOG_SPY )
        {
            log_info( "Signal catched !" );
        }
        pthread_cond_broadcast( &spies.onTurnAction );
        pthread_cond_broadcast( &case_officer_thread.onTurnAction );
    }
    pthread_cond_broadcast( &spies.onTurnAction );
    pthread_cond_broadcast( &case_officer_thread.onTurnAction );

    for ( i = 0; i < NB_OF_SPIES; i++ )
    {
        int retval = pthread_join( tid[i], NULL );
        if ( retval != 0 )
        {
            handleFatalError( "Thread join error\n" );
        }
    }
    if ( pthread_join( tidCaseOfficer, NULL ) != 0 )
    {
        handleFatalError( "Thread join error case officer\n" );
    }
    deleteSpiesManager( shm );
    deleteCaseOfficerThreadStorage();
}
