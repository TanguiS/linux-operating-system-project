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
 * @brief /!\ source, manage the actions of the case officer in the city /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "case_officer/action_case_officer.h"
#include "case_officer/case_officer_manager.h"
#include "common/logger.h"
#include "enemy_country/enemy_country_manager.h"
#include "utils/other.h"
#include "utils/path.h"
#include <math.h>
#include <mqueue.h>
#include <signal.h>

extern thread_storage_t case_officer_thread;
extern memory_t* memory;
extern volatile bool runnable;

static void manageRetrieveMessage( case_officer_pargs_t* args, memory_t* memory )
{
    int i;
    if ( memory->mailbox_nb_of_msgs > 0 )
    {
        for ( i = args->nbOfMessages; i < memory->mailbox_nb_of_msgs + args->nbOfMessages; i++ )
        {
            memcpy( &args->mailBoxMessages[i], &memory->mailbox_content[i], sizeof( message_t ) );
            if ( ENABLE_LOG_CASE_OFFICER )
            {
                log_green( "RETRIEVED MESSAGE %s", args->mailBoxMessages[i].content );
            }
            memset( &memory->mailbox_content[i], '\0', sizeof( message_t ) );
        }
        args->nbOfMessages = memory->mailbox_nb_of_msgs + args->nbOfMessages;
    }
    memory->mailbox_nb_of_msgs = 0;
}

static void manageSendMessage( case_officer_pargs_t* args )
{
    int i, res;
    mqd_t mq = mq_open( MESSAGE_QUEUE_NAME, O_WRONLY, 0666, NULL );

    for ( i = 0; i < args->nbOfMessages; i++ )
    {
        res = mq_send( mq, args->mailBoxMessages[i].content, MAX_MESSAGE_LENGTH, args->mailBoxMessages[i].priority );
        if ( res == -1 && ENABLE_LOG_CASE_OFFICER )
        {
            log_red( "ERROR WHILE SENDING MESSAGE" );
        }
        if ( ENABLE_LOG_CASE_OFFICER )
        {
            log_green( "SENDING MESSAGE %s", args->mailBoxMessages[i].content );
        }
        memset( &args->mailBoxMessages[i], '\0', sizeof( message_t ) );
    }
    mq_close( mq );
    args->nbOfMessages = 0;
}

static void manageHomeAction( case_officer_t* case_officer )
{
    point_t destination;
    if ( compare_points( case_officer->location, case_officer->home ) )
    {
        if ( ENABLE_LOG_CASE_OFFICER )
        {
            log_yellow( "ALREADY AT HOME! : (%d %d)", case_officer->location.column, case_officer->location.row );
        }
        return;
    }

    destination = getRandomNextPointFromDrandGenerator( case_officer->location, case_officer->home, memory->map.cells, &case_officer_thread.randBuffer );
    if ( ENABLE_LOG_CASE_OFFICER )
    {
        log_yellow( "MOVED TO HOME! : from (%d %d) to (%d, %d)", case_officer->location.column, case_officer->location.row, destination.column, destination.row );
    }
    updatePopulation( case_officer->location, destination, memory->map.cells );
    case_officer->location = destination;
}

static void manageSuperMarketAction( case_officer_t* case_officer )
{
    point_t destination = getClosestSuperMarket( case_officer->location, memory->map.cells );

    if ( destination.row == ERROR )
    {
        manageHomeAction( case_officer );
        return;
    }
    if ( compare_points( case_officer->location, destination ) )
    {
        return;
    }
    destination = getRandomNextPointFromDrandGenerator( case_officer->location, destination, memory->map.cells, &case_officer_thread.randBuffer );
    if ( ENABLE_LOG_CASE_OFFICER )
    {
        log_red( "MOVED TO MARKET : (%d %d) to (%d, %d)", case_officer->location.column, case_officer->location.row, destination.column, destination.row );
    }
    updatePopulation( case_officer->location, destination, memory->map.cells );
    case_officer->location = destination;
}

static void manageMailBoxAction( case_officer_t* case_officer, case_officer_pargs_t* args, memory_t* memory, double timer )
{
    if ( compare_points( case_officer->location, case_officer->mailbox ) )
    {
        if ( timer >= args->timeSecondVisitMailBox )
        {
            args->secondPassageDone = true;
        }
        args->firstPassageDone = true;
        if ( ENABLE_LOG_CASE_OFFICER )
        {
            log_red( "I AM AT MAILBOX !" );
        }
        // kill( memory->characters.counter_officers[0].myPid, SIGUSR2 );
        manageRetrieveMessage( args, memory );
        return;
    }
    point_t destination = getRandomNextPointFromDrandGenerator( case_officer->location, case_officer->mailbox, memory->map.cells, &case_officer_thread.randBuffer );
    updatePopulation( case_officer->location, destination, memory->map.cells );
    case_officer->location = destination;
}

static void updateTime( double currentTime, double* timeToUpdate, double start, double end )
{
    if ( fabs( currentTime - end ) < LIMIT )
    {
        ( *timeToUpdate ) = getRandomTimeBetween( start, end, &case_officer_thread.randBuffer );
    }
}

static void updateTimes( case_officer_pargs_t* args, double currentTime )
{
    updateTime( currentTime, &args->timeSuperMarket, START_SUPERMARKET_TIME, END_SUPERMARKET_TIME );
    updateTime( currentTime, &args->timeFirstVisitMailBox, START_DAY_TIME, END_DAY_TIME );
    updateTime( currentTime, &args->timeSecondVisitMailBox, args->timeFirstVisitMailBox, END_DAY_TIME );
    updateTime( currentTime, &args->timeMessage, START_MESSAGE_TIME, END_MESSAGE_TIME );
    if ( currentTime < LIMIT && ENABLE_LOG_CASE_OFFICER )
    {
        log_green( "TIMES FOR TODAY :" );
        log_green( "FIRST MAILBOX : %f", args->timeFirstVisitMailBox );
        log_green( "SECOND MAILBOX : %f", args->timeSecondVisitMailBox );
        log_green( "SUPERMARKET : %f", args->timeSuperMarket );
        log_green( "MESSAGES : %f", args->timeMessage );
    }
}

static void updateSuperMarket( case_officer_pargs_t* args, double currentTime )
{
    if ( args->isGoingToSuperMarket && currentTime >= END_SUPERMARKET_TIME )
    {
        args->isGoingToSuperMarket = false;
    }
    else if ( !args->isGoingToSuperMarket && currentTime >= args->timeSuperMarket && currentTime <= END_SUPERMARKET_TIME )
    {
        args->isGoingToSuperMarket = true;
    }
}

static void updateMailBox( case_officer_pargs_t* args, double currentTime )
{
    if ( currentTime >= END_DAY_TIME )
    {
        args->firstPassageDone = false;
        args->secondPassageDone = false;
        args->isGoingToMailBox = false;
        return;
    }
    if ( !args->isGoingToMailBox && currentTime >= args->timeFirstVisitMailBox && !args->firstPassageDone )
    {
        args->isGoingToMailBox = true;
    }
    if ( !args->isGoingToMailBox && currentTime >= args->timeSecondVisitMailBox && !args->secondPassageDone )
    {
        args->isGoingToMailBox = true;
    }
    if ( args->firstPassageDone && currentTime <= args->timeSecondVisitMailBox )
    {
        args->isGoingToMailBox = false;
    }
    if ( args->secondPassageDone )
    {
        args->isGoingToMailBox = false;
    }
}

static void updateMessage( case_officer_pargs_t* args, double currentTime )
{
    args->isSendingMessages = fabs( currentTime - args->timeMessage ) < LIMIT;
}

static void updateActions( case_officer_pargs_t* args, double currentTime )
{
    updateSuperMarket( args, currentTime );
    updateMailBox( args, currentTime );
    updateMessage( args, currentTime );
}

static void manageTurnAction( case_officer_pargs_t* args )
{
    double timer = memory->simulationTime;
    updateTimes( args, timer );
    updateActions( args, timer );
    if ( ENABLE_LOG_CASE_OFFICER )
    {
        log_magenta( "Time = %f", timer );
    }
    case_officer_t* memory_officer = memory->characters.case_officers;
    if ( args->isGoingToSuperMarket )
    {
        if ( ENABLE_LOG_CASE_OFFICER )
        {
            log_green( "MARKET TIME ! : (%d %d)", memory_officer->location.column, memory_officer->location.row );
        }
        manageSuperMarketAction( memory_officer );
        return;
    }
    if ( args->isGoingToMailBox )
    {
        if ( ENABLE_LOG_CASE_OFFICER )
        {
            log_red( "MAILBOX TIME !" );
        }
        manageMailBoxAction( memory_officer, args, memory, timer );
        return;
    }
    if ( args->isSendingMessages )
    {
        manageSendMessage( args );
        return;
    }
    manageHomeAction( memory_officer );
}

void* handleCaseOfficerThreadCall( void* vargp )
{
    case_officer_pargs_t* args = (case_officer_pargs_t*)vargp;
    while ( runnable )
    {
        pthread_cond_wait( &case_officer_thread.onTurnAction, &case_officer_thread.mutex );
        if ( !runnable )
        {
            return NULL;
        }
        pthread_mutex_lock( &case_officer_thread.accessMemory );
        manageTurnAction( args );
        pthread_mutex_unlock( &case_officer_thread.accessMemory );
    }
    log_magenta( "END" );
    return NULL;
}