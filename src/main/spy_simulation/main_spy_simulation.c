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
 * @file main_spy_simulation.c
 * @brief /!\ exec, manage the spy simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "common/logger.h"
#include "memory/memory.h"
#include "spy_simulation/handle_simulation.h"
#include "timer/handle_timer_signal.h"
#include <unistd.h> /* TEST */

int shm;

int main( int argc, char** argv )
{
    double timeInSec;
    memory_t* memory;

    if ( ENABLE_GLOBAL_LOGS )
    {
        log_debug( "Process Spy Simulation is running..." );
    }

    handleSpySimulationParameters( argc, argv, &timeInSec );
    initHandlerEndSimulation();
    initHandlerAlarmTurn();
    shm = initSharedMemory( SHM_NAME );

    memory = loadMemory( shm );

    runSimulation( timeInSec );

    // sleep( RAND_MAX );

    deleteSharedMemory( memory );
    unloadMemory( memory );
    closeMemory( shm );

    if ( ENABLE_GLOBAL_LOGS )
    {
        log_debug( "Process Spy Simulation is done!!" );
    }

    exit( EXIT_SUCCESS );
    return EXIT_SUCCESS;
}