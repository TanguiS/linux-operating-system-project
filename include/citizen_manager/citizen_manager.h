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
 * @file citizen_manager.h
 * @brief /!\ header, manage the citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __CITIZEN_MANAGER_HH__
#define __CITIZEN_MANAGER_HH__

#include "memory/memory.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct thread_storage
{
    pthread_t citizens[NB_OF_CITIZENS];
    pthread_mutex_t accessMemory;
    pthread_mutex_t mutex[NB_OF_CITIZENS];
    pthread_cond_t onTurnAction;
    struct drand48_data randBuffer;
} thread_storage_t;

typedef struct id_citizen_thread
{
    int idCitizen;
    bool goToMarket;
    bool flipCoin;
    double marketHours;
    int* updateState;
} citizen_pargs_t;

/**
 * @brief Run the Citizen Simulation
 * 
 */
void runCitizenManager();

#endif /* __CITIZEN_MANAGER_HH__ */