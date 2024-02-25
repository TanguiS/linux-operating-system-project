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
 * @file action_spy_network.h
 * @brief /!\ function declarations which manage global spy actions /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#ifndef __ACTION_SPY_NETWORK_HH__
#define __ACTION_SPY_NETWORK_HH__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>    /* fork() */
#include <signal.h>    /* sigaction() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */
#include <string.h>
#include <string.h>

#include "./memory/memory.h"
#include "./enemy_spy_network/create_spy_network.h"
#include "./enemy_spy_network/spies_arguments.h"
#include "./enemy_spy_network/spies_manager.h"
#include "utils/other.h"
#include "./utils/path.h"
#include "common/logger.h"




#define DAY_BEGIN 8.
#define DAY_END 17.
#define NIGHT_BEGIN 20.
#define MIDNIGHT 0.0
#define NIGHT_END 3.0
#define MORNING_BEGIN 5.0

#define PROBA_MARKET 0.1
#define PROBA_HOME 0.3
#define PROBA_CHOOSE_COMPANY 0.6


extern thread_storage_spies spies;

/**
 * @brief Handle spies threads after their are created. It launch threads every turns
*/
void * handleSpiesThreadCall( void * args );

/**
 * @brief Manage one spy to go to in his home
 * @param args
*/
void manageHome( spies_pargs * args );

/**
 * @brief Give a random double value between 0 and 1(thread safe)
*/
double makeRandom();

/**
 * @brief Give a free point close to the reference
 * 
 * @param args 
*/
void getCloseTo( spies_pargs * args );

/**
 * @brief Update the number of turn needed to move for a spy and to heal
 * 
 * @param args
*/
void updateTurnMove( spies_pargs * args );

/**
 * @brief Move a spy if he can. It consider his health points. 
*/
void manageMoving( spies_pargs * args, point_t final  );


#endif