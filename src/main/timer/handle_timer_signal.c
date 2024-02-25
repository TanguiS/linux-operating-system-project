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
 * @file handle_timer_signal.c
 * @brief /!\ source, manage the sync with the timer /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "timer/handle_timer_signal.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "spy_simulation/handle_turn_action.h"
#include "timer/manage_timer.h"
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TIMER_BIN "./bin/timer.out" /**< String name & directories for timer exec */

typedef struct sigaction sigaction_t; /**< shortcut */
volatile sig_atomic_t runnable = 1;   /**< Define volatile global variable to stop the process when finished */

/**
 * @brief Handle SIGTERM signal sended by the timer
 * 
 * @param signal 
 */
void handleEndSimulationSignal( int signal )
{
    if ( signal == SIGTERM )
    {
        if ( ENABLE_LOG_TIMER )
        {
            log_debug( "/!\\ SIGTERM is called by TIMER /!\\ : Safe delete spy network and kill the simulation" );
        }
        runnable = 0;
    }
}

void initHandlerEndSimulation()
{
    sigaction_t endSimulation;
    memset( &endSimulation, '\0', sizeof( endSimulation ) );
    endSimulation.sa_handler = &handleEndSimulationSignal;
    sigaction( SIGTERM, &endSimulation, NULL );
}

void initHandlerAlarmTurn()
{
    sigaction_t timer;
    memset( &timer, '\0', sizeof( timer ) );
    timer.sa_handler = &handleTimerAlarm; /**< Function from parent process to deal the SIGALARM signal */
    sigaction( SIGALRM, &timer, NULL );
}

void handleTimerAlarm( int signal )
{
    if ( signal == SIGALRM )
    {
        if ( ENABLE_LOG_TIMER )
        {
            log_debug( "\nNew Round and/or End of the previous one" );
        }
        handleTurnAction();
    }
}

void createAndRunTimer( pid_t pidSimulation, double timeInSec )
{
    char strPidSimulation[255];
    char strTimeInsec[255];
    sprintf( strPidSimulation, "%d", pidSimulation );
    sprintf( strTimeInsec, "%f", timeInSec );
    execl( TIMER_BIN, TIMER_BIN, strPidSimulation, strTimeInsec, NULL );
}