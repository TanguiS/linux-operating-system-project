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
 * @file perform_turn_action.h
 * @brief /!\ source, manage the instruction to perform the current turn of the simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "spy_simulation/handle_turn_action.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"

void handleTurnAction()
{
    if ( ENABLE_GLOBAL_LOGS )
    {
        log_debug( "Spy Simulation is doing some important stuff at the time ..." );
    }
}