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
 * @file spies_manager.h
 * @brief /!\ function declarations which manage spies in every turn /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#ifndef __SPIES_MANAGER_HH__
#define __SPIES_MANAGER_HH__

#include "memory/memory.h"
#include <pthread.h>

typedef struct thread_storage_spies
{
    pthread_t spies[NB_OF_SPIES];
    pthread_mutex_t accessMemory;
    pthread_mutex_t mutex[NB_OF_SPIES];
    pthread_cond_t onTurnAction;
    struct drand48_data randBuffer;
} thread_storage_spies;

/**
 * @brief Handle spies every turn 
*/
void runSpiesManager();

#endif
