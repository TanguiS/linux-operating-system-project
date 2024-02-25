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
 * @file main_counter_simulation.c
 * @brief /!\ exec, manage the counter intelligence officer /!\
 * @author SENG Thomas <thomas.seng@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "common/logger.h"
#include "counter_intelligence_officer/action_counter_intelligence_officer.h"
#include "utils/common_signals.h"
#include <signal.h>

extern volatile bool runnable;

/**
 * @brief Initialises the handler for SIGUSR1 signal
 */
static void initHandlerUsr1Signal()
{
    struct sigaction fighting_action;
    memset( &fighting_action, '\0', sizeof( fighting_action ) );
    fighting_action.sa_handler = &handlerUsr1Signal;
    sigaction( SIGUSR1, &fighting_action, NULL );
}

/**
 * @brief Initialises the handler for SIGUSR2 signal
 */
static void initHandlerUsr2Signal()
{
    struct sigaction start_fighting_action;
    memset( &start_fighting_action, '\0', sizeof( start_fighting_action ) );
    start_fighting_action.sa_handler = &handlerUsr2Signal;
    sigaction( SIGUSR2, &start_fighting_action, NULL );
}

int main()
{
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
    {
        log_debug( "Process Counter Intelligence Officer manager is running..." );
    }

    initHandlerContSignal();
    initHandlerEndSignal();
    initHandlerUsr1Signal();
    initHandlerUsr2Signal();

    runCounterIntelligenceOfficerManager();

    exit( EXIT_SUCCESS );
    return EXIT_SUCCESS;
}