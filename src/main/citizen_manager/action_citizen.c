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
 * @file action_citizen.c
 * @brief /!\ source, manage the different action for a citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "citizen_manager/action_citizen.h"
#include "citizen_manager/citizen_manager.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include "utils/other.h"
#include "utils/path.h"
#include <math.h> /* TEST NEED TO BE REMOVED */
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

extern thread_storage_t citizens;
extern memory_t* memory;
extern volatile bool runnable;

#define PROBA_MARKET 0.25 /*<-- Probality to go to the Market in the day */
#define CITIZEN_ID_TEST 2 /*<-- For log to see what is going on */

/**
 * @brief do the id given in arg go to the market today
 *
 * @return true
 * @return false
 */
static bool doIGoToMarket( int idCitizen );

/**
 * @brief Get the Time Of Market object
 *
 * @return double
 */
static double getTimeOfMarket();

/**
 * @brief is the given Citizen's id is working at the market
 *
 * @param idCitizen
 * @return true
 * @return false
 */
static bool isMarketBoy( int idCitizen );

/**
 * @brief In the case it is time to work, the function is mmoving the citizen to his working place
 *
 * @param idCitizen
 * @param citizen all the citizen of the simulation
 * @param updateState
 * @param seed
 */
static void manageWorkAction( int idCitizen, citizen_t* citizen, int* updateState );

/**
 * @brief In the case it is time to be home, the function is moving the citizen to his home
 *
 * @param idCitizen
 * @param citizen
 * @param updateState
 * @param seed
 */
static void managerHomeAction( int idCitizen, citizen_t* citizen, int* updateState );

/**
 * @brief In the case it is time to be at the market, the function is moving the citizen to the the market
 *
 * @param idCitizen
 * @param citizen
 * @param updateState
 * @param seed
 */
static void manageMarketAction( int idCitizen, citizen_t* citizen, int* updateState );

/**
 * @brief Mange the turn to decide what action to do according to the current datas.
 *
 * @param args
 */
static void manageTurnAction( citizen_pargs_t* args );

/**
 * @brief Update the state of the citizens
 *
 * @param citizen
 * @param updateState
 * @param newState
 */
static void changeState( citizen_t* citizen, int* updateState, state_e newState );

/**
 * @brief Move the citizen and update the citizen according to his destination
 *
 * @param citizen
 * @param destination
 * @param updateState
 */
static void move( citizen_t* citizen, point_t destination, int* updateState );

static bool doIGoToMarket( int idCitizen )
{
    double rand;
    drand48_r( &citizens.randBuffer, &rand );
    if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
    {
        log_bright_magenta( "Rand to go market : %.2f", rand );
    }
    return ( rand <= PROBA_MARKET );
}

static double getTimeOfMarket()
{
    double rand;
    drand48_r( &citizens.randBuffer, &rand );
    return market_begin + rand * ( market_end - market_begin );
}

static bool isMarketBoy( int idCitizen )
{
    return getCellFromPoint( memory->characters.citizens[idCitizen].working_place, memory->map.cells )->type == SUPERMARKET;
}

static void changeState( citizen_t* citizen, int* updateState, state_e newState )
{
    --updateState[citizen->currentState];
    citizen->currentState = newState;
    ++updateState[citizen->currentState];
}

static void move( citizen_t* citizen, point_t destination, int* updateState )
{
    updatePopulation( citizen->location, destination, memory->map.cells );
    citizen->location = destination;
    if ( citizen->currentState != INDEX_MOVING )
    {
        changeState( citizen, updateState, INDEX_MOVING );
    }
}

static void manageWorkAction( int idCitizen, citizen_t* citizen, int* updateState )
{
    point_t destination;
    if ( compare_points( citizen->location, citizen->working_place ) )
    {
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_red( "I am at work !, status = %d, id:%d", citizen->currentState, idCitizen );
        }
        changeState( citizen, updateState, INDEX_WORK );
        return;
    }
    destination = getRandomNextPointFromDrandGenerator( citizen->location, citizen->working_place, memory->map.cells, &citizens.randBuffer );
    if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
    {
        log_green( "Citizen moved to work: previously here : %d %d, now here %d %d, sattus = %d id:%d", citizen->location.column, citizen->location.row, destination.column, destination.row, citizen->currentState, idCitizen );
    }
    move( citizen, destination, updateState );
}

static void managerHomeAction( int idCitizen, citizen_t* citizen, int* updateState )
{
    point_t destination;
    if ( compare_points( citizen->location, citizen->home ) )
    {
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_yellow( "I am at my home !, status = %d, id:%d", citizen->currentState, idCitizen );
        }
        changeState( citizen, updateState, INDEX_HOME );
        return;
    }
    destination = getRandomNextPointFromDrandGenerator( citizen->location, citizen->home, memory->map.cells, &citizens.randBuffer );
    if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
    {
        log_green( "Citizen moved to home: previously here : %d %d, now here %d %d, status = %d, id:%d", citizen->location.column, citizen->location.row, destination.column, destination.row, citizen->currentState, idCitizen );
    }
    move( citizen, destination, updateState );
}

static void manageMarketAction( int idCitizen, citizen_t* citizen, int* updateState )
{
    point_t destination;
    destination = getClosestSuperMarket( citizen->location, memory->map.cells );
    if ( compare_points( citizen->location, destination ) )
    {
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_magenta( "I am at a market !, status = %d, id:%d", citizen->currentState, idCitizen );
        }
        changeState( citizen, updateState, INDEX_HOME );
        return;
    }
    if ( destination.row == ERROR )
    {
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_info( "The market must be crowded...., status = %d", citizen->currentState );
        }
        managerHomeAction( idCitizen, citizen, updateState );
        return;
    }
    destination = getRandomNextPointFromDrandGenerator( citizen->location, destination, memory->map.cells, &citizens.randBuffer );
    if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
    {
        log_green( "Citizen moved to market: previously here : %d %d, now here %d %d, status = %d id:%d", citizen->location.column, citizen->location.row, destination.column, destination.row, citizen->currentState, idCitizen );
    }
    move( citizen, destination, updateState );
}

static void manageTurnAction( citizen_pargs_t* args )
{
    int idCitizen = args->idCitizen;
    bool* goToMarket = &args->goToMarket;
    bool* flipCoin = &args->flipCoin;
    double* marketHour = &args->marketHours;
    int* updateState = args->updateState;
    double timer = memory->simulationTime;
    citizen_t* citizen = &memory->characters.citizens[idCitizen];

    if ( ( timer >= work_begin && timer < work_end ) || ( timer >= work_begin && timer < work_end_market_boyz && isMarketBoy( idCitizen ) ) )
    {
        manageWorkAction( idCitizen, citizen, updateState );
        return;
    }
    if ( *flipCoin && timer >= market_begin && timer < market_end )
    {
        *goToMarket = doIGoToMarket( idCitizen );
        *marketHour = getTimeOfMarket();
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_debug( "i flipped the coin and the result is .................. : " );
        }
        if ( *goToMarket && ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_debug( "I decided to go to market today at the time : %f!!", *marketHour );
        }
        else if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_debug( "I decided not to go to the market today... no food for me..." );
        }
        *flipCoin = false;
    }
    if ( timer >= market_begin && timer < market_end && *goToMarket && timer >= *marketHour )
    {
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_debug( "I am going to the market! !!" );
        }
        manageMarketAction( idCitizen, citizen, updateState );
        return;
    }
    if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
    {
        log_debug( "I am going home !!" );
    }
    if ( timer > market_end && !( *flipCoin ) )
    {
        if ( ENABLE_LOG_CITIZENS && idCitizen == CITIZEN_ID_TEST )
        {
            log_debug( "I am availabe to flip the coin again" );
        }
        *flipCoin = true;
        *goToMarket = false;
    }
    managerHomeAction( idCitizen, citizen, updateState );
}

void* handleCitizensThreadCall( void* vargp )
{
    int id = ( (citizen_pargs_t*)vargp )->idCitizen;
    while ( runnable )
    {
        pthread_cond_wait( &citizens.onTurnAction, &citizens.mutex[id] );
        if ( !runnable )
        {
            return NULL;
        }
        if ( ENABLE_LOG_CITIZENS && id == CITIZEN_ID_TEST )
        {
            pthread_mutex_lock( &citizens.accessMemory );
            // memory->simulationTime = fmod( memory->simulationTime+0.166666667, 24. );
            log_debug( "Hi I am a thread and i have the id citizen : %d, my am i a market ? : %d at the time %f\n", id, isMarketBoy( id ), memory->simulationTime );
            fflush( stdout );
            pthread_mutex_unlock( &citizens.accessMemory );
        }
        pthread_mutex_lock( &citizens.accessMemory );
        manageTurnAction( (citizen_pargs_t*)vargp );
        pthread_mutex_unlock( &citizens.accessMemory );
    }
    return NULL;
}