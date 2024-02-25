#include "counter_intelligence_officer/counter_intelligence_officer_manager.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "counter_intelligence_officer/action_counter_intelligence_officer.h"
#include "counter_intelligence_officer/cameras.h"
#include "memory/memory.h"
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

#define COUNTER memory->characters.counter_officers[0]

#define INPUT 1
#define OUTPUT 0

memory_t* memory;
volatile bool runnable = 1;

/**
 * @brief Delete nicely the counter intelligence officer manager
 *
 * @param shm
 */
static void deleteCounterIntelligenceOfficerManager( int shm )
{
    unloadMemory( memory );
    closeMemory( shm );
}

/**
 * @brief Create cameras network
 *
 * @param suspiciousIdPipe
 */
static pid_t createCameras( int suspiciousIdPipe[2] )
{
    pid_t pid;
    pid = fork();
    if ( pid == -1 )
    {
        handleFatalError( "[ERROR pid]" );
        return EXIT_FAILURE;
    }
    if ( pid > 0 )
    {
        return pid;
    }
    runCameras( suspiciousIdPipe );
    return 0;
}

/**
 * @brief Create the storage of the counter intelligence officer lifetime
 */
static counter_intelligence_officer_lifetime_t createLifetime()
{
    counter_intelligence_officer_lifetime_t newLifetime;

    newLifetime.day_of_research = false;
    newLifetime.isScoutingCity = true;
    newLifetime.isTrackingSuspect = false;
    newLifetime.isReturningToCityHall = false;
    newLifetime.looksForMailbox = false;
    newLifetime.knowsMailBoxLocation = false;
    newLifetime.possibleMailboxLocation.column = -1;
    newLifetime.possibleMailboxLocation.row = -1;
    newLifetime.previousPoint.column = -1;
    newLifetime.previousPoint.row = -1;

    return newLifetime;
}

void runCounterIntelligenceOfficerManager()
{
    int shm;
    int suspiciousIdPipe[2];
    int flags;
    shm = openMemory( SHM_NAME );
    memory = loadMemory( shm );
    srand( time( NULL ) );

    COUNTER.myPid = getpid();

    counter_intelligence_officer_lifetime_t counterLifetime = createLifetime();

    if ( pipe( suspiciousIdPipe ) == -1 )
    {
        handleFatalError( "[ERROR pipe]" );
    }
    counterLifetime.pidCameras = createCameras( suspiciousIdPipe );
    flags = fcntl( suspiciousIdPipe[OUTPUT], F_GETFL, 0 );
    if ( flags == -1 )
    {
        handleFatalError( "perror[FLAGS]" );
    }
    if ( fcntl( suspiciousIdPipe[OUTPUT], F_SETFL, flags | O_NONBLOCK ) == -1 )
    {
        handleFatalError( "perror[NONBLOCK fcntl]" );
    }
    close( suspiciousIdPipe[INPUT] );
    counterLifetime.suspiciousIdPipe = suspiciousIdPipe[OUTPUT];

    /****************************************************/
    /***************** Section de test ******************/
    /****************************************************/
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
    {
        log_debug( " The counter intelligence officer start at (%d, %d)\n\
    The location of the city hall is (%d, %d)\n",
                   COUNTER.location.row, COUNTER.location.column, COUNTER.city_hall.row, COUNTER.city_hall.column );
    }
    /****************************************************/
    /*************** Fin section de test ****************/
    /****************************************************/
    if ( ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER )
    {
        log_yellow( "Simulation begins! Let's fight till the end!" );
    }

    while ( runnable )
    {
        pause();
        if ( !runnable )
        {
            return;
        }
        if ( COUNTER.canFightBegin )
        {
            if ( COUNTER.fightInProgress )
            {
                manageFightAction();
            }
            COUNTER.canFightBegin = false;
        }
        else
        {
            manageTurnAction( &counterLifetime );
        }
    }

    kill( counterLifetime.pidCameras, SIGUSR2 );
    wait( NULL );

    if ( counterLifetime.knowsMailBoxLocation )
    {
        memory->simulation_has_ended = 2;
    }
    else
    {
        memory->simulation_has_ended = 3;
    }

    deleteCounterIntelligenceOfficerManager( shm );
    exit( EXIT_SUCCESS );
}