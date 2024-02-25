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
 * @file action_counter_intelligence_officer.c
 * @brief /!\ source, manage the different action for a counter_intelligence_officer in the city /!\
 * @author SENG Thomas <thomas.seng@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "counter_intelligence_officer/action_counter_intelligence_officer.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "counter_intelligence_officer/counter_intelligence_officer_manager.h"
#include "data.h"
#include "memory/memory.h"
#include "utils/other.h"
#include "utils/path.h"
#include <signal.h>
#include <unistd.h>

#define MAX_HP 10

extern memory_t* memory;
extern counter_intelligence_officer_lifetime_t args;
struct drand48_data randBuffer;

#define COUNTER_ID_TEST 1
#define PROBA_MAILBOX 70
#define PROBA_SHOOT_FIRST 0.9
#define PROBA_FLEE 0.9
#define START_OF_DAY 8
#define END_OF_DAY 17
#define EPSILON 0.005
#define COUNTER memory->characters.counter_officers[0]

/**
 * @brief Checks if the counter intelligence officer has found the mailbox
 *
 * @param days
 */
static bool doIFindTheMailBox( int days )
{
    return ( ( rand() % 10 ) <= ( PROBA_MAILBOX + 1 * days ) );
}

/**
 * @brief Checks if the counter intelligence officer is in the building where mailbox is
 */
static bool isMailBoxHere()
{
    return compare_points( COUNTER.location, memory->characters.case_officers[0].mailbox );
}

/**
 * @brief Checks if the target is in the next case
 *
 * @param args
 */
static bool isTargetVisible( counter_intelligence_officer_lifetime_t* args )
{
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
    {
        log_red( "Suspect location : (%d, %d)", args->suspectLocation->row, args->suspectLocation->column );
        log_red( "Counter location : (%d, %d)", COUNTER.location.row, COUNTER.location.column );
        if ( isValidePoints( *args->suspectLocation ) ) {
            log_green( "isTargetVisible result: %d", compare_points( *args->suspectLocation, getNextPoint( COUNTER.location, *args->suspectLocation, memory->map.cells ) ) );
        }
    }
    if ( isValidePoints( COUNTER.location ) && isValidePoints( *( args->suspectLocation ) ) )
    {
        return compare_points( *( args->suspectLocation ), getNextPoint( COUNTER.location, *( args->suspectLocation ), memory->map.cells ) );
    }
    return 0;
}

/**
 * @brief Moves the counter intelligence officer location to the next point toward the suspect
 *
 * @param args
 */
static void goToSuspectLocation( counter_intelligence_officer_lifetime_t* args )
{
    int i;
    args->previousPoint = COUNTER.location;
    for ( i = 0; i < 3; i++ )
    {
        if ( !isTargetVisible( args ) )
        {
            COUNTER.location = getNextPoint( COUNTER.location, *args->suspectLocation, memory->map.cells );
        }
    }
    updatePopulation( args->previousPoint, COUNTER.location, memory->map.cells );
}

/**
 * @brief Checks if target is in a residential
 *
 * @param args
 */
static bool isTargetInAResidential( counter_intelligence_officer_lifetime_t* args )
{
    if ( memory->map.cells[args->suspectLocation->row][args->suspectLocation->column].type == RESIDENTIAL_BUILDING ) {
        return true;
    }
    return false;
}

/**
 * @brief Moves the counter intelligence location location to the next point toward the city hall
 */
static point_t goBackToCityHall()
{
    point_t targetPoint = COUNTER.location;
    targetPoint = getNextPoint( targetPoint, COUNTER.city_hall, memory->map.cells );
    updatePopulation( COUNTER.location, targetPoint, memory->map.cells );
    return targetPoint;
}

/**
 * @brief Moves the counter intelligence location location to the next point toward the possible mailbox location
 *
 * @param args
 */
static point_t goToPossibleMailbox( counter_intelligence_officer_lifetime_t* args )
{
    point_t targetPoint = COUNTER.location;
    targetPoint = getNextPoint( targetPoint, args->possibleMailboxLocation, memory->map.cells );
    updatePopulation( COUNTER.location, targetPoint, memory->map.cells );
    return targetPoint;
}

/**
 * @brief Moves the counter intelligence location location to the next point toward the mailbox
 */
static point_t goToMailBox()
{
    point_t targetPoint = COUNTER.location;
    targetPoint = getNextPoint( targetPoint, COUNTER.mailbox_location, memory->map.cells );
    updatePopulation( COUNTER.location, targetPoint, memory->map.cells );
    return targetPoint;
}

/**
 * @brief Manages the action that allows to follow suspect
 *
 * @param args
 * @param timer
 */
static void manageVerificationAction( counter_intelligence_officer_lifetime_t* args, double timer )
{
    if ( isTargetVisible( args ) )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_magenta( "I am behind the suspect ! It's %lf", timer );
        }
        if ( isTargetInAResidential( args ) )
        {
            if ( timer <= START_OF_DAY - EPSILON || timer >= END_OF_DAY + EPSILON )
            {
                args->possibleMailboxLocation = *args->suspectLocation;
            }
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_green( "It's enough for today, I will be back later..." );
            }
            args->isTrackingSuspect = false;
            args->isReturningToCityHall = true;
        }
    }
    else
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_debug( "Counter is not arrived ..." );
        }
        goToSuspectLocation( args );
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_green( "Counter intelligence officer moved to suspect: previously here : %d %d, now here %d %d",
                       args->previousPoint.column, args->previousPoint.row, COUNTER.location.column, COUNTER.location.row );
            log_green( "The suspect is here: %d %d",
                       args->suspectLocation->column, args->suspectLocation->row );
        }
    }
    return;
}

/**
 * @brief Communication with cameras
 *
 * @param args
 */
static point_t* handleCameraInformation( counter_intelligence_officer_lifetime_t* args )
{
    static point_t noSuspect = { -1, -1 };
    int reader = -1;
    read( args->suspiciousIdPipe, &reader, sizeof( COUNTER.targeted_character_id ) );
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER ) {
        log_bright_magenta( "Reader is ............ : %d", reader );
    }
    if ( isValidePoints( *getPointFromId( reader, &memory->characters ) ) )
    {
        COUNTER.targeted_character_id = reader;
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_info( "Dans handleCameraInfo : %d", reader );
            log_info( "Dans handleCameraInfo : %d", COUNTER.targeted_character_id );
        }
        args->isTrackingSuspect = true;
        return getPointFromId( COUNTER.targeted_character_id, &memory->characters );
    }
    args->isTrackingSuspect = false;
    COUNTER.targeted_character_id = -1;
    return &noSuspect;
}

/**
 * @brief Checks if the counter intelligence officer engages the fight or flees
 */
static bool doIFlee()
{
    return ( rand() % 10 ) == 0 ? false : true;
}

/**
 * @brief The counter intelligence officer goes back to city hall after an injury and the simalution ends
 */
static void fleeUntilTheEnd()
{
    if ( compare_points( COUNTER.location, COUNTER.city_hall ) )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_magenta( "simulation_has_ended value has been changed, this is the end..." );
        }
        memory->simulation_has_ended = 1;
    }
    else
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_magenta( "Counter officer is fleeing..." );
        }
        COUNTER.location = goBackToCityHall();
    }
}

/**
 * @brief Manages the counter intelligence officer actions before he discovers the mailbox location
 *
 * @param args
 */
static void manageWorkAtCityHallBeforeMailBox( counter_intelligence_officer_lifetime_t* args )
{
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER ) {
        log_bright_magenta( "kill( args->pidCameras, SIGUSR1 ); = kill( %d, SIGUSR1 );", args->pidCameras );
    }
    kill( args->pidCameras, SIGUSR1 );
    args->suspectLocation = handleCameraInformation( args );
    if ( isValidePoints( *( args->suspectLocation ) ) )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_green( "A suspect has been detected! His location is: (%d, %d)", args->suspectLocation->row, args->suspectLocation->column );
        }
        args->isScoutingCity = 0;
        args->isTrackingSuspect = 1;
    }
}

/**
 * @brief The counter intelligence officer copies the messages of the mailbox
 *
 * @param args
 */
static void manageCopyMessage( counter_intelligence_officer_lifetime_t* args )
{
    int i;
    if ( memory->mailbox_nb_of_msgs > 0 )
    {
        for ( i = 0; i < memory->mailbox_nb_of_msgs; i++ )
        {
            memcpy( &( COUNTER.copiedMessages[i] ), &memory->mailbox_content[i], sizeof( message_t ) );
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_magenta( "RETRIEVED MESSAGE %s", COUNTER.copiedMessages[i].content );
            }
        }
        COUNTER.nb_of_messages = memory->mailbox_nb_of_msgs;
    }
    args->looksForMailbox = false;
    args->isReturningToCityHall = true;
}

/**
 * @brief Manages the counter intelligence officer actions after he discovers the mailbox location
 *
 * @param args
 * @param timer
 * @param targetTime
 */
static void manageWorkAtCityHallAfterMailBox( counter_intelligence_officer_lifetime_t* args, double timer, double targetTime )
{
    if ( targetTime - EPSILON < timer )
    {
        args->looksForMailbox = true;
    }

    if ( targetTime + EPSILON < timer )
    {
        args->looksForMailbox = false;
    }

    if ( args->looksForMailbox && !args->doICheckMailbox )
    {
        if ( compare_points( COUNTER.location, COUNTER.mailbox_location ) )
        {
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_red( "Counter is arrived to the mailbox!" );
            }
            manageCopyMessage( args );
            args->doICheckMailbox = true;
        }
        else
        {
            COUNTER.location = goToMailBox();
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_red( "Counter is going to mailbox, his location is: (%d, %d)", COUNTER.location.row, COUNTER.location.column );
            }
        }
    }
    else
    {
        if ( !compare_points( COUNTER.location, COUNTER.city_hall ) )
        {
            args->isReturningToCityHall = true;
        }
    }
}

/**
 * @brief Checks if the possible mailbox location is the right location
 *
 * @param args
 */
static void manageBuildingInspection( counter_intelligence_officer_lifetime_t* args )
{
    if ( !args->knowsMailBoxLocation )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_yellow( "Is mailbox here ? %d", isMailBoxHere() );
        }
        if ( isMailBoxHere() )
        {
            if ( doIFindTheMailBox( args->day_of_research ) )
            {
                args->knowsMailBoxLocation = true;
            }
            else
            {
                args->day_of_research++;
                args->isReturningToCityHall = true;
            }
            if ( args->knowsMailBoxLocation && ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_error( "Mailbox has been found!" );
            }
            if ( !args->knowsMailBoxLocation && ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_error( "Mailbox has not been found yet, we'll be back tomorow..." );
            }
        }
        else
        {
            if ( args->day_of_research == 3 )
            {
                if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
                {
                    log_error( "We were wrong, there was nothing here..." );
                }
                args->possibleMailboxLocation.column = -1;
                args->possibleMailboxLocation.row = -1;
                args->day_of_research = 0;
            }
            else
            {
                if ( !args->knowsMailBoxLocation && ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
                {
                    log_error( "Mailbox has not been found yet, we'll be back tomorow..." );
                }
                args->day_of_research++;
                args->looksForMailbox = false;
            }
            args->isReturningToCityHall = true;
        }
    }
    if ( args->knowsMailBoxLocation )
    {
        COUNTER.mailbox_location = args->possibleMailboxLocation;
        manageCopyMessage( args );
    }
    args->doICheckMailbox = true;
}

/**
 * @brief Provides a random time between two hours
 *
 * @param end
 * @param start
 */
static double getCounterRandomTimeBetween( double start, double end )
{
    return start + (double)( rand() % (int)( ( end - start ) * NUMBER_TURNS_IN_HOUR ) ) / TIME_TURN;
}

void manageTurnAction( counter_intelligence_officer_lifetime_t* args )
{
    double timer = memory->simulationTime;
    double whenDoIGoCheckMailbox;
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
    {
        log_red( "Counter intelligence officer is awaken ... It's %lf!", timer );
        log_info( "targeted_character_id: %d", COUNTER.targeted_character_id );
    }

    if ( ( timer >= START_OF_DAY - EPSILON ) && timer <= ( START_OF_DAY + EPSILON ) )
    {

        whenDoIGoCheckMailbox = getCounterRandomTimeBetween( START_OF_DAY, END_OF_DAY );
        args->doICheckMailbox = false;
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_yellow( "It's a new day! Counter officer can check the mailbox! If I do it, I will go at %lf", whenDoIGoCheckMailbox );
        }
    }

    if ( COUNTER.health_points != MAX_HP )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_error( "Counter officer is hit! He lost the game..." );
        }
        fleeUntilTheEnd();
        return;
    }

    if ( args->isReturningToCityHall )
    {
        if ( compare_points( COUNTER.location, COUNTER.city_hall ) )
        {
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_yellow( "Counter officer is back to the City Hall!" );
            }
            args->isReturningToCityHall = false;
            args->isScoutingCity = true;
            return;
        }
        else
        {
            COUNTER.location = goBackToCityHall();
            return;
        }
    }

    if ( args->isScoutingCity && !args->knowsMailBoxLocation )
    {
        if ( !isValidePoints( args->possibleMailboxLocation ) )
        {
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_yellow( "Counter officer is scouting the city, he has not found the mailbox for the moment..." );
            }
            manageWorkAtCityHallBeforeMailBox( args );
            return;
        }
        else
        {
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_yellow( "A possible mailbox has been found, let's check this..." );
            }
            args->looksForMailbox = true;
            args->isScoutingCity = false;
            return;
        }
    }

    if ( args->isScoutingCity && args->knowsMailBoxLocation )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_yellow( "Counter officer is scouting the city, and he has found the mailbox!" );
        }
        manageWorkAtCityHallAfterMailBox( args, timer, whenDoIGoCheckMailbox );
        return;
    }

    if ( args->isTrackingSuspect )
    {
        if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
        {
            log_yellow( "Counter officer is tracking suspect..." );
        }
        manageVerificationAction( args, timer );
        return;
    }

    if ( args->looksForMailbox && !args->doICheckMailbox )
    {
        if ( !compare_points( COUNTER.location, args->possibleMailboxLocation ) )
        {
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_yellow( "Counter officer goes to a possible mailbox..." );
            }
            COUNTER.location = goToPossibleMailbox( args );
        }
        else
        {
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_red( "Counter officer is arrived to a possible mailbox, verification is in progress..." );
            }
            manageBuildingInspection( args );
        }

        return;
    }
}

void handlerUsr1Signal( int signal )
{
    if ( signal == SIGUSR1 )
    {
        COUNTER.health_points--;
        log_info( "Got the SIGUSR1 signal !\nRemaining HP : %d", COUNTER.health_points );
        fleeUntilTheEnd();
    }
}

void handlerUsr2Signal( int signal )
{
    if ( signal == SIGUSR2 )
    {
        COUNTER.canFightBegin = true;

        if ( compare_points( COUNTER.location, memory->characters.case_officers[0].mailbox ) )
        {
            COUNTER.fightInProgress = ( rand() % 10 != 0 );
            if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
            {
                log_info( "SIGUSR2 is cought! Is the fight engaged ? %d", COUNTER.fightInProgress );
            }
        }
    }
}

/**
 * @brief Manage the variables that define the actions in a fight
 */
static void fightWithSpy()
{
    COUNTER.firstShoot = ( rand() % 10 ) == 0 ? false : true;
    COUNTER.didIHitTarget = ( rand() % 10 ) == 0 ? false : true;

    if ( COUNTER.health_points != MAX_HP )
    {
        fleeUntilTheEnd();
    }

    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
    {
        log_info( "Counter officer shot at someone!" );
    }

    if ( COUNTER.doubleDamage )
    {
        kill( COUNTER.targetPid, SIGUSR2 );
    }
    else
    {
        kill( COUNTER.targetPid, SIGUSR1 );
    }
}

void manageFightAction()
{
    if ( doIFlee() )
    {
        COUNTER.fightInProgress = 0;
    }
    else
    {
        fightWithSpy();
    }
    return;
}