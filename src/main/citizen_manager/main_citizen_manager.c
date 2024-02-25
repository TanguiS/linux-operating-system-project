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
 * @file main_citizen_manager.c
 * @brief /!\ exec, manage the citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "citizen_manager/citizen_manager.h"
#include "citizen_manager/create_citizen.h"
#include "common/logger.h"
#include "utils/common_signals.h"

int main( void )
{
    if ( ENABLE_LOG_CITIZENS )
    {
        log_debug( "Process Citizen manager is running..." );
    }
    initHandlerContSignal();
    initHandlerEndSignal();
    runCitizenManager();
    exit( EXIT_SUCCESS );
    return EXIT_SUCCESS;
}