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
 * @author SENG Thomas <thomas.seng@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __COUNTER_MANAGER_HH__
#define __COUNTER_MANAGER_HH__

#include "memory/memory.h"
#include <stdbool.h>

/* typedef struct thread_storage {
    pthread_t citizens[NB_OF_CITIZENS];
    pthread_mutex_t accessMemory;
    pthread_mutex_t mutex[NB_OF_CITIZENS];
    pthread_cond_t onTurnAction;
} thread_storage_t;
 */

/**
 * @brief Stocks information during the lifetime of the counter intelligence officer
 */
typedef struct counter_intelligence_officer_lifetime
{
    int day_of_research;
    int suspiciousIdPipe;
    pid_t pidCameras;
    bool isScoutingCity;
    bool isTrackingSuspect;
    bool isReturningToCityHall;
    bool looksForMailbox;
    bool knowsMailBoxLocation;
    bool doICheckMailbox;
    point_t* suspectLocation;
    point_t previousPoint;
    point_t possibleMailboxLocation;
} counter_intelligence_officer_lifetime_t;

/**
 * @brief Executes the counter intelligence officer manager
 */
void runCounterIntelligenceOfficerManager();

#endif /* __COUNTER_MANAGER_HH__ */