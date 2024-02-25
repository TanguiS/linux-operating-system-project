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
 * @file day_action_spies.c
 * @brief /!\ function which manage spies actions during the day /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "./enemy_spy_network/day_action_spies.h"
#include "./utils/path.h"
#include "utils/other.h"
#include <math.h>
#include <stdbool.h>

extern memory_t* memory;
extern spy_t* spy;

/**
 * @brief Inform if a spy will go to a market
 * 
 * @return bool
*/
static bool doIGoToMarket() {
    double rand = makeRandom();
    return ( rand <= PROBA_MARKET );
}

/**
 * @brief Inform if a spy will go to his home
 * 
 * @return bool
*/
static bool doIGoToHome() {
    double rand = makeRandom();
    return ( rand <= PROBA_HOME );
}


/**
 * @brief Manage a spy to go to in a market
 * 
 * @param args
*/
static void manageMarket( spies_pargs * args ) {
    point_t market;
    market = getClosestSuperMarket( spy->location, memory->map.cells );
    if ( compare_points( spy->location, market ) )
    {
        if ( spy->id == 1 && ENABLE_LOG_SPY )
        {
            log_debug( "I am in the market : %d\n", spy->id );
        }
        return;
    }
    if ( market.row == ERROR )
    {
        args->day_objective = HOME;
        manageHome( args );
        return;
    }
    manageMoving( args, market );
}


/**
 * @brief Give the next company to steal
 * 
 * @param args
 * @param position
 * @return company_t
*/
static company_t getCompagny( spies_pargs * args, int position ) {
    int i, notStolen = 0;
    for ( i = 0; i < NB_OF_COMPANY; i++ )
    {
        if ( args->list_of_companies_visited[i] == false )
        {
            notStolen++;
        }
        if ( notStolen == position )
        {
            return memory->companies[i];
        }
    }
    return memory->companies[0];
}

/**
 * @brief Set a company to steal for the spy
 * 
 * @param args
*/
static void chooseCompany( spies_pargs * args ) {
    double rand;
    int position_company;
    rand = makeRandom();
    position_company = ( (int)( rand * 100 ) ) % ( NB_OF_COMPANY - spy->nb_of_stolen_companies );
    args->company_to_steal = getCompagny( args, position_company );
    getCloseTo( args );
    if ( spy->id == 1 && ENABLE_LOG_SPY )
    {
        log_yellow( "the point is in %d %d by %d", args->point_closed_company.row, args->point_closed_company.column, spy->id );
        log_green( "the company could be on %d %d", args->company_to_steal.cell.row, args->company_to_steal.cell.column );
    }
}

/**
 * @brief Manage a spy to spy during the day
 * 
 * @param args
*/
static void manageSpying( spies_pargs * args ) {
    double rand;
    if ( args->observation_turn_number == -1 )
    {
        chooseCompany( args );
        args->observation_turn_number = 0;
    }
    if ( args->id_spy == 1 && ENABLE_LOG_SPY )
        log_debug( "the conmpany is on %d %d", args->company_to_steal.cell.row, args->company_to_steal.cell.column );

    if ( compare_points( spy->location, args->point_closed_company ) )
    {
        if ( args->id_spy == 1 && ENABLE_LOG_SPY )
        {
            log_debug( "Spy %d is observing %d", args->id_spy, args->observation_turn_number );
        }
        if ( args->observation_turn_number == 12 )
        {
            if ( ENABLE_LOG_SPY )
            {
                log_debug( "Spy %d will steal %d", args->id_spy, args->time_to_steal );
            }
            rand = makeRandom();
            if ( rand <= 0.85 )
            {
                args->night_objective = STEAL;
            }
            else
            {
                args->flag_to_steal_next_night = true;
            }
            args->day_objective = HOME;
            manageHome( args );
        }
        else
        {
            getCloseTo( args );
            args->observation_turn_number++;
        }
        return;
    }

    manageMoving( args, args->point_closed_company );
}


void chooseDayAction(  double timer,int idSpy, spies_pargs *spy_arg ) {

    if ( spy->health_points_has_changed == true ) {
        updateTurnMove( spy_arg );
        spy->health_points_has_changed = false;
    }
    if ( timer - floor( timer ) < 0.05 )
    {
        if ( idSpy == 1 && ENABLE_LOG_SPY )
        {
            log_debug( "Spy %d change is action", idSpy );
        }
        if ( doIGoToMarket() )
        {
            spy_arg->day_objective = MARKET;
        }
        else if ( doIGoToHome() )
        {
            spy_arg->day_objective = HOME;
        }
        else
        {
            spy_arg->day_objective = SPYING;
        }
    }

    if ( idSpy == 1 && ENABLE_LOG_SPY )
    {
        log_debug( "Spy %d is %d", idSpy, spy_arg->day_objective );
        log_debug( "Spy %d is on the case : %d %d", idSpy, spy->location.column, spy->location.row );
    }

    if ( spy_arg->day_objective == MARKET )
    {
        manageMarket( spy_arg );
        return;
    }
    else if ( spy_arg->day_objective == HOME )
    {
        manageHome( spy_arg );
        return;
    }
    else
    {
        if ( spy_arg->observation_turn_number == 12 )
        {
            spy_arg->day_objective = HOME;
            manageHome( spy_arg );
            return;
        }
        manageSpying( spy_arg );
        return;
    }
}