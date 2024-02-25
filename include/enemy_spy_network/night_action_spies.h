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
 * @file night_action_spies.h
 * @brief /!\ function declarations which manage spies actions during the night /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#ifndef __NIGHT_ACTION_SPIES_HH__
#define __NIGHT_ACTION_SPIES_HH__

#include "./enemy_spy_network/action_spy_network.h"
#include "./enemy_spy_network/spies_manager.h"

/**
 * @brief Choose an action amoung several actions
 * 
 * @param timer
 * @param spy_arg
*/
void chooseNightAction(  double timer, spies_pargs *spy_arg );


#endif