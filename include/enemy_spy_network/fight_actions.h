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
 * @file fight_actions.h
 * @brief /!\ function declarations which manage spies fight with the counter officer /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#ifndef __FIGHT_ACTION_HH__
#define __FIGHT_ACTION_HH__

#include "./memory/memory.h"

/**
 * @brief Initialize the handler's function  for the signal SIGUSR1
*/
void initHandlerUsr1Signal();

/**
 * @brief Initialize the handler's function  for the signal SIGUSR2
*/
void initHandlerUsr2Signal();


#endif