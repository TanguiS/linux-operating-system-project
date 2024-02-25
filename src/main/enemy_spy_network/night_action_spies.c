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
 * @file night_action_spies.c
 * @brief /!\ function which manage spies actions during the night /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "enemy_spy_network/night_action_spies.h"
#define TROMPER "MANON DES SOURCES, MANON DES SOURCES, MANON DES SOURCES, MANON DES SOURCES, MANON DES SOURCES  "

extern memory_t* memory;
extern spy_t* spy;

/**
 * @brief Get the mailbox point
 * 
 * @return point_t
*/
static point_t getMailbox() {
    return memory->characters.counter_officers[0].mailbox_location;
}


/**
 * @brief Manage a message which will be send 
 * 
 * @param spy_args
*/
static void manageMessage( spies_pargs * spy_args ) {
    point_t mailbox = getMailbox();
    if ( spy->id == 1 && ENABLE_LOG_SPY )
    {
        log_magenta( " la mail box est en %d %d", mailbox.row, mailbox.column );
    }
    if ( compare_points( spy->location, mailbox ) )
    {
        if ( spy->id == 1 && ENABLE_LOG_SPY )
        {
            log_magenta( " je passe ici" );
        }

        strcpy( memory->mailbox_content[memory->mailbox_nb_of_msgs].content, spy_args->message.content );
        memory->mailbox_content[memory->mailbox_nb_of_msgs].priority = spy_args->message.priority;
        memory->mailbox_nb_of_msgs++;
        spy_args->has_a_message = false;
        //kill( memory->characters.counter_officers->myPid, SIGUSR2 );
        spy_args->night_objective = GO_BACK_HOME;
        return;
    }
    manageMoving( spy_args, mailbox );
    spy_args->has_a_message = false;
}


/**
 * @brief Informs if a spy can steal at this time
 * 
 * @param timer
 * @param rand
 * @param spy_args
*/
static void autoriseToSteal( double timer, double rand, spies_pargs * spy_args ) {
    if ( ( ( timer >= DAY_END ) && (timer < NIGHT_BEGIN ) ) && ( rand <= 0.02 ) ) {
        spy_args->can_i_steal = true;
        return;
    }
    if ( ( timer >= NIGHT_BEGIN ) && ( rand <= 0.1 ) )
    {
        spy_args->can_i_steal = true;
        return;
    }
    if ( ( timer < NIGHT_END ) && ( rand <= 0.76 ) )
    {
        spy_args->can_i_steal = true;
        return;
    }
    if ( ( ( timer >= NIGHT_END ) && ( timer < MORNING_BEGIN ) ) && ( rand <= 0.1 ) )
    {
        spy_args->can_i_steal = true;
        return;
    }
    if ( ( ( timer >= MORNING_BEGIN ) && ( timer < DAY_BEGIN ) ) && ( rand <= 0.02 ) )
    {
        spy_args->can_i_steal = true;
        return;
    }
}

/**
 * @brief Notify when a period is changing 
 * 
 * @param timer
 * @param spy_args
 * @return bool
*/
static bool changePeriode( const double timer, spies_pargs * spy_args ) {
    if ( timer - DAY_END <= 0.05 ) {
        spy_args->can_i_steal = false;
        return true;
    }
    if ( timer - NIGHT_BEGIN <= 0.5 )
    {
        spy_args->can_i_steal = false;
        return true;
    }
    if ( timer - MIDNIGHT <= 0.05 )
    {
        spy_args->can_i_steal = false;
        return true;
    }
    if ( timer - NIGHT_END <= 0.05 )
    {
        spy_args->can_i_steal = false;
        return true;
    }
    if ( timer - MORNING_BEGIN <= 0.05 )
    {
        spy_args->can_i_steal = false;
        return true;
    }
    return false;
}


/**
 * @brief Generate a valid message
 * 
 * @param args
*/
static void generateMessage( spies_pargs * args ) {
    int i;
    double rand = makeRandom();
    for ( i = 0; i < MAX_MESSAGE_LENGTH; i++ )
    {
        args->message.content[i] = (int)( rand * 100 ) % 26 + 65;
    }
}

/**
 * @brief Set the message priority 
 * 
 * @param args
*/
static void getPriority( spies_pargs * args ) {
    double rand = makeRandom();
    if ( rand < 0.01 )
    {
        args->message.priority = 10;
        return;
    }
    if ( rand < 0.06 )
    {
        args->message.priority = 9;
        return;
    }
    if ( rand < 0.2 )
    {
        args->message.priority = 6;
        return;
    }
    if ( rand < 0.5 )
    {
        args->message.priority = 3;
        return;
    }
    args->message.priority = 2;
    return;
}


/**
 * @brief Generate a message with text and priority 
 * 
 * @param args
*/
static void getMessage( spies_pargs * args ) {
    generateMessage( args );
    getPriority( args );
}


/**
 * @brief Generate a fake message
 * 
 * @param args
*/
static void getFakeMessage( spies_pargs * args ) {
    strcpy( args->message.content, TROMPER );
    args->message.priority = 0;
}


/**
 * @brief Updates a company when it has been stolen 
 * 
 * @param args
*/
static void updateCompany( spies_pargs * args ) {
    int i;
    for ( i = 0; i < NB_OF_COMPANY; i++ )
    {
        if ( compare_points( args->company_to_steal.cell, memory->companies[i].cell ) == true )
        {
            args->list_of_companies_visited[i] = true;
        }
    }
}


/**
 * @brief Last action when the spy is stealing 
 * 
 * @param args
*/
static void finishStealing( spies_pargs * args ) {
    double rand = makeRandom();
    args->stealing_turn_number = 0;
    if ( rand <= 0.9 )
    {
        getMessage( args );
        updateCompany( args );
        args->can_i_steal = false;
        return;
    }
    getFakeMessage( args );
}


/**
 * @brief Manage stealing actions 
 * 
 * @param spy_args
 * @param timer
*/
static void manageStealing( spies_pargs * spy_args, const double timer ) {

    double rand;
    if ( spy_args->can_i_steal == false || changePeriode( timer, spy_args ) == true )
    {
        rand = makeRandom();
        autoriseToSteal( timer, rand, spy_args );
        manageHome( spy_args );
        return;
    }
    if ( compare_points( spy->location, spy_args->company_to_steal.cell ) )
    {
        if ( spy_args->stealing_turn_number == 6 )
        {
            finishStealing( spy_args );
            spy_args->night_objective = MAILBOX;
            spy_args->stealing_turn_number = 0;
            spy_args->observation_turn_number = -1;
            return;
        }

        spy_args->stealing_turn_number++;
        if ( spy->id == 1 && ENABLE_LOG_SPY )
        {
            log_yellow( " %d number of turn", spy_args->stealing_turn_number );
        }
        return;
    }
    manageMoving( spy_args, spy_args->company_to_steal.cell );
}

void chooseNightAction( double timer, spies_pargs* spy_arg )
{
    if ( spy_arg->flag_to_steal_next_night == true && ( timer - DAY_END ) < 0.05 )
    {
        spy_arg->want_to_steal = true;
        spy_arg->flag_to_steal_next_night = false;
        spy_arg->night_objective = MAILBOX;
        getFakeMessage( spy_arg );
    }
    else if ( spy_arg->want_to_steal == true && ( timer - DAY_END ) < 0.05 )
    {
        spy_arg->night_objective = STEAL;
        spy_arg->want_to_steal = false;
    }

    if ( spy_arg->night_objective == GO_BACK_HOME )
    {
        if ( spy->id == 1 && ENABLE_LOG_SPY )
        {
            log_yellow( " I go home" );
        }
        manageHome( spy_arg );
        return;
    }
    if ( spy_arg->night_objective == MAILBOX )
    {
        if ( spy->id == 1 && ENABLE_LOG_SPY )
        {
            log_yellow( " I am sending the mail" );
        }
        manageMessage( spy_arg );
        return;
    }
    if ( spy_arg->night_objective == STEAL )
    {
        if ( spy->id == 1 && ENABLE_LOG_SPY )
        {
            log_yellow( " I am stealing" );
        }
        manageStealing( spy_arg, timer );
        return;
    }
}