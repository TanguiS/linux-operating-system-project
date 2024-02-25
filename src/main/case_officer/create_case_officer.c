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
 * @file create_case_officer.h
 * @brief /!\ source, manage the creation of the case officer in the city /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "case_officer/create_case_officer.h"
#include "case_officer/action_case_officer.h"
#include "case_officer/case_officer_manager.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "enemy_country/enemy_country_manager.h"
#include "memory/memory.h"
#include "utils/other.h"

extern thread_storage_t case_officer_thread;

static void initCaseOfficerThreadStorage()
{
    if ( pthread_mutex_init( &case_officer_thread.mutex, NULL ) != 0 )
    {
        handleFatalError( "Mutex init error\n" );
    }
    if ( pthread_mutex_init( &case_officer_thread.accessMemory, NULL ) != 0 )
    {
        handleFatalError( "Mutex init error\n" );
    }
    if ( pthread_cond_init( &case_officer_thread.onTurnAction, NULL ) != 0 )
    {
        handleFatalError( "Cond init error\n" );
    }
}

void createCaseOfficerThread( pthread_t* tid, case_officer_pargs_t* pargs )
{
    initCaseOfficerThreadStorage();
    srand48_r( time( NULL ), &case_officer_thread.randBuffer );
    mqd_t mq;
    mq_unlink( MESSAGE_QUEUE_NAME );                         // clear the message queue
    mq = mq_open( MESSAGE_QUEUE_NAME, O_CREAT, 0666, NULL ); // recreate the message queue
    mq_close( mq );
    *tid = 0;
    pargs->altercation = false;
    pargs->injury = NO_INJURY;
    pargs->timeFirstVisitMailBox = getRandomTimeBetween( START_DAY_TIME, END_DAY_TIME, &case_officer_thread.randBuffer );
    pargs->timeSecondVisitMailBox = getRandomTimeBetween( pargs->timeFirstVisitMailBox, END_DAY_TIME, &case_officer_thread.randBuffer );
    pargs->timeSuperMarket = getRandomTimeBetween( START_SUPERMARKET_TIME, END_SUPERMARKET_TIME, &case_officer_thread.randBuffer );
    pargs->timeMessage = getRandomTimeBetween( START_MESSAGE_TIME, END_MESSAGE_TIME, &case_officer_thread.randBuffer );
    pargs->isSendingMessages = false;
    pargs->isGoingToMailBox = false;
    pargs->isGoingToSuperMarket = false;
    pargs->firstPassageDone = false;
    pargs->secondPassageDone = false;
    pargs->nbOfMessages = 0;
    if ( pthread_create( tid, NULL, handleCaseOfficerThreadCall, pargs ) != 0 )
    {
        handleFatalError( "Thread creation error\n" );
    }
}

void deleteCaseOfficerThreadStorage()
{
    if ( pthread_mutex_destroy( &case_officer_thread.mutex ) != 0 )
    {
        handleFatalError( "Mutex destroy error\n" );
    }
    if ( pthread_mutex_destroy( &case_officer_thread.accessMemory ) != 0 )
    {
        handleFatalError( "Mutex destroy error\n" );
    }
    if ( pthread_cond_destroy( &case_officer_thread.onTurnAction ) != 0 )
    {
        handleFatalError( "Cond destroy error\n" );
    }
}