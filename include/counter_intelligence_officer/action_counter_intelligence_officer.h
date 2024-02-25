#ifndef __ACTION_COUNTER_NETWORK_H__
#define __ACTION_COUNTER_NETWORK_H__

#include "counter_intelligence_officer/counter_intelligence_officer_manager.h"
#include "memory/memory.h"
#include "utils/path.h"

/**
 * @brief Handle the SIGUSR1 signal
 *
 * @param signal
 */
void handlerUsr1Signal( int signal );

/**
 * @brief Update the SIGUSR2 signal
 *
 * @param signal
 */
void handlerUsr2Signal( int signal );

/**
 * @brief Manage all the fight action
 */
void manageFightAction();

/**
 * @brief Manage the actions the counter intelligence officer does in a turn
 *
 * @param args
 */
void manageTurnAction( counter_intelligence_officer_lifetime_t* args );

#endif