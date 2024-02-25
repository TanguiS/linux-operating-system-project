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
 * @file fight_actions.c
 * @brief /!\ function which manage spies fight with the counter officer /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "./enemy_spy_network/fight_actions.h"
#include "common/logger.h"
#include "utils/common_signals.h"
#include <signal.h>

extern memory_t* memory;
extern sig_atomic_t beginFight;
extern sig_atomic_t is_double_domage;


/**
 * @brief Counter attack the counter officer 
*/
static void counterAttack() {
    kill( memory->characters.counter_officers->myPid, SIGUSR1 );
}

/**
 * @brief handler function for SIGUSR1
 * 
 * @brief signal
*/
static void inform_fight_sign1( int signal ) {
    int id;
    spy_t* spy;
    if ( signal == SIGUSR1 )
    {
        id = memory->characters.counter_officers->targeted_character_id;
        if ( ( id < 0 || id > 3 ) && ENABLE_LOG_SPY )
        {
            log_green( "the id shot is not correct " );
            return;
        }
        spy = &memory->characters.spies[id];
        spy->health_points_has_changed = true;
        if ( memory->characters.counter_officers->firstShoot == false )
        {
            counterAttack();
            return;
        }
        if ( memory->characters.counter_officers->didIHitTarget == false )
        {
            counterAttack();
            return;
        }
        spy->health_points -= 1;
        spy->health_points_has_changed = true;
        beginFight = true;
        is_double_domage = false;
    }
}

/**
 * @brief handler function for SIGUSR1
 * 
 * @brief signal
*/
static void inform_fight_sign2( int signal ) {
    int id;
    spy_t* spy;
    if ( signal == SIGUSR2 )
    {
        id = memory->characters.counter_officers->targeted_character_id;
        if ( ( id < 0 || id > 3 ) && ENABLE_LOG_SPY )
        {
            log_green( "the id shot is not correct " );
            return;
        }
        spy = &memory->characters.spies[id];
        spy->health_points -= 1;
        spy->health_points_has_changed = true;
        beginFight = true;
        is_double_domage = false;
    }
}

void initHandlerUsr1Signal()
{
    struct sigaction shootWeak;
    memset( &shootWeak, '\0', sizeof( shootWeak ) );
    shootWeak.sa_handler = &inform_fight_sign1;
    sigaction( SIGUSR1, &shootWeak, NULL );
}

void initHandlerUsr2Signal()
{
    struct sigaction shootStrong;
    memset( &shootStrong, '\0', sizeof( shootStrong ) );
    shootStrong.sa_handler = &inform_fight_sign2;
    sigaction( SIGUSR2, &shootStrong, NULL );
}
