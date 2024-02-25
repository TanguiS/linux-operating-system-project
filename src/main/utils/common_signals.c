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
 * @file signal_citizen.c
 * @brief /!\ source, manage the citizen signal to process the turn /!\
 * @author Kylian Picque <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "utils/common_signals.h"
#include "common/logger.h"
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct sigaction sigaction_t;

extern volatile bool runnable;

void initHandlerContSignal()
{
    sigaction_t startTurn;
    memset( &startTurn, '\0', sizeof( startTurn ) );
    startTurn.sa_handler = &handlerContSignal; /**< Function from parent process to deal the SIGTERM signal */
    sigaction( SIGCONT, &startTurn, NULL );
}

void handlerContSignal( int signal )
{
    if ( signal == SIGCONT )
    {
        /*         if ( ENABLE_LOG ) {
            log_debug( "Signal SIGCONT is catched !" );
        } */
    }
}

void initHandlerEndSignal()
{
    sigaction_t endSimulation;
    memset( &endSimulation, '\0', sizeof( endSimulation ) );
    endSimulation.sa_handler = &handlerEndSignal; /**< Function from parent process to deal the SIGTERM signal */
    sigaction( SIGTERM, &endSimulation, NULL );
}

void handlerEndSignal( int signal )
{
    if ( signal == SIGTERM )
    {
        runnable = 0;
        if ( ENABLE_LOG_TIMER )
        {
            log_debug( "Signal SIGTERM is catched !" );
        }
    }
}