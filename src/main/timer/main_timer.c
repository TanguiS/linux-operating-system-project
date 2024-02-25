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
 * @file manage_timer.c
 * @brief /!\ exec, manage the timer in order to sync the simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include <stdlib.h>

#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "timer/manage_timer.h"

#define NUMBER_TURN 2016 /**< Number of Turn in the simulation, 1 turn = 10 fictive min */

int main( int argc, char** argv )
{
    double timeInSec;
    pid_t pidSimulation;

    if ( ENABLE_LOG_TIMER )
    {
        log_debug( "Process timer is running..." );
    }

    handleTimerParameters( argc, argv, &pidSimulation, &timeInSec );
    runTimer( timeInSec, NUMBER_TURN, pidSimulation );

    if ( ENABLE_LOG_TIMER )
    {
        log_debug( "Process timer is Done !" );
    }

    exit( EXIT_SUCCESS );
    return EXIT_SUCCESS;
}