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
 * @file create_spy_network.h
 * @brief /!\ function declarations which creat spy threads /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#ifndef CREAT_SPY_NETWORK_HH
#define CREAT_SPY_NETWORK_HH

#include <stdio.h>
#include <stdlib.h>

#include "./enemy_spy_network/action_spy_network.h"
#include "./enemy_spy_network/spies_arguments.h"
#include "common/handle_fatal_error.h"
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */
#include <unistd.h>

#include "spies_manager.h"

/**
 * @brief create threads which represent spies
 * 
 * @param tid
 * @param pargs
*/
void createSpiesThreads( pthread_t* tid, spies_pargs * pargs );

/**
 * @brief delete thread which represented spies
*/
void deleteSpiesThreadStorage();

#endif