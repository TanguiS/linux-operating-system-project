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
 * @file handle_simulation.h
 * @brief /!\ header, manage the spy simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __HANDLE_SIMULATION_HH__
#define __HANDLE_SIMULATION_HH__

#include "../timer/handle_timer_signal.h"
#include "memory/memory.h"

typedef struct pid_storage
{
    pid_t citizenManager;
    pid_t counterIntelligenceOfficer;
    pid_t enemyCountry;
    pid_t enemySpyNetwork;
    pid_t monitor;
} pid_storage_t;

int initSharedMemory( const char* shm );

void deleteSharedMemory( memory_t* mem );

/**
 * @brief Run the Spy Simulation
 * 
 * @param timeInSec 
 */
void runSimulation( double timeInSec );

/**
 * @brief Manage a spySimulation classical turn
 * 
 */
void manageCurrentSimulationTurn();

void manageEndSimulation();

/**
 * @brief Check the parameters & convert them
 * @todo PUT IT SOMEWHERE ELSE
 * 
 * @param argc 
 * @param argv 
 * @param resultTimeInSec string from argv to double
 */
void handleSpySimulationParameters( int argc, char** argv, double* resultTimeInSec );

#endif /* __HANDLE_SIMULATION_HH__ */