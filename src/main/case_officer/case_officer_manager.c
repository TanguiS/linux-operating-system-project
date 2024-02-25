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
 * @file action_case_officer.h
 * @brief /!\ source, manage the case officer in the city /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "case_officer/case_officer_manager.h"
#include "case_officer/action_case_officer.h"
#include "case_officer/create_case_officer.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

thread_storage_t case_officer_thread;
extern memory_t* memory;
extern bool runnable;

static void deleteCaseOfficerManager( int shm )
{
    unloadMemory( memory );
    closeMemory( shm );
    deleteCaseOfficerThreadStorage();
}

void runCaseOfficerManager()
{
    int shm;
    pthread_t tid;
    case_officer_pargs_t pargs;
    shm = openMemory( SHM_NAME );
    memory = loadMemory( shm );

    createCaseOfficerThread( &tid, &pargs );

    while ( runnable )
    {
        pause();
        pthread_cond_broadcast( &case_officer_thread.onTurnAction );
    }
    pthread_cond_broadcast( &case_officer_thread.onTurnAction );

    if ( pthread_join( tid, NULL ) != 0 )
    {
        handleFatalError( "Thread join error\n" );
    }
    deleteCaseOfficerManager( shm );
}