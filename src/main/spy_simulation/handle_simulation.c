/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

/**
 * @file handle_simulation.c
 * @brief /!\ source, manage the spy simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "spy_simulation/handle_simulation.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "timer/manage_timer.h"
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_INT -1                 /**< Error ID */
#define IS_CHILD( bool ) bool == 0 /**< Detect which process is the Child */
#define CITIZEN_MANAGER_BIN "./bin/citizen_manager.out"
#define ENEMY_SPY_NETWORK_BIN "./bin/enemy_spy_network.out"
#define ENEMY_COUNTRY_BIN "./bin/enemy_country.out"
#define COUNTER_INTELLIGENCE_OFFICER_MANAGER_BIN "./bin/counter_intelligence_officer.out"
#define MONITOR_BIN "./bin/monitor.out"

extern volatile sig_atomic_t runnable; /**< Extern global variable runnable to update the simulation when finished */
extern volatile sig_atomic_t endTurn;
volatile pid_storage_t pidStorage;

static pid_t createCitizenManager()
{
    pid_t citizenManagerPid;
    citizenManagerPid = fork();
    if ( citizenManagerPid == ERR_INT )
    {
        handleFatalError( "Error [fork(citizen_manager)]: " );
    }
    if ( IS_CHILD( citizenManagerPid ) )
    {
        execl( CITIZEN_MANAGER_BIN, CITIZEN_MANAGER_BIN, NULL );
    }
    pidStorage.citizenManager = citizenManagerPid;
    return citizenManagerPid;
}

static pid_t createSpyNetwork()
{
    pid_t spyNetworkPid = fork();
    if ( spyNetworkPid == ERR_INT )
    {
        handleFatalError( "Error [fork(spy_network)]:" );
    }
    if ( IS_CHILD( spyNetworkPid ) )
    {
        execl( ENEMY_SPY_NETWORK_BIN, ENEMY_SPY_NETWORK_BIN, NULL );
    }
    pidStorage.enemySpyNetwork = spyNetworkPid;
    return spyNetworkPid;
}

static pid_t createEnemyCountry()
{
    pid_t enemyCountryPid = fork();
    if ( enemyCountryPid == ERR_INT )
    {
        handleFatalError( "Error [fork(spy_network)]:" );
    }
    if ( IS_CHILD( enemyCountryPid ) )
    {
        execl( ENEMY_COUNTRY_BIN, ENEMY_COUNTRY_BIN, NULL );
    }
    pidStorage.enemyCountry = enemyCountryPid;
    return enemyCountryPid;
}

static pid_t createCounterIntelligenceOfficerManager()
{
    pid_t counterManagerPid;
    counterManagerPid = fork();
    if ( counterManagerPid == ERR_INT )
    {
        handleFatalError( "Error [fork(counter_intelligence_officer_manager)]: " );
    }
    if ( IS_CHILD( counterManagerPid ) )
    {
        execl( COUNTER_INTELLIGENCE_OFFICER_MANAGER_BIN, COUNTER_INTELLIGENCE_OFFICER_MANAGER_BIN, NULL );
    }
    pidStorage.counterIntelligenceOfficer = counterManagerPid;
    return counterManagerPid;
}

static pid_t createMonitor()
{
    pid_t monitorPid;
    monitorPid = fork();
    if ( monitorPid == ERR_INT )
    {
        handleFatalError( "Error [fork(monitor)]: " );
    }
    if ( IS_CHILD( monitorPid ) )
    {
        execl( MONITOR_BIN, MONITOR_BIN, NULL );
        return 0;
    }
    pidStorage.monitor = monitorPid;
    return monitorPid;
}

static pid_t createTimer( pid_t pidSimulation, double timeInSec )
{
    pid_t timerPid;
    timerPid = fork();
    if ( timerPid == ERR_INT )
    {
        handleFatalError( "Error [fork()]: " );
    }
    if ( IS_CHILD( timerPid ) )
    {
        createAndRunTimer( pidSimulation, timeInSec );
    }
    return timerPid;
}

void handleSpySimulationParameters( int argc, char** argv, double* resultTimeInSec )
{
    if ( argc != 2 )
    {
        handleFatalError( "\n\nNot Enough argument : nee time in sec between each turn, format : xxx.xx \n\n" );
    }
    *resultTimeInSec = strtod( argv[1], NULL );
}

int initSharedMemory( const char* shm )
{
    return createMemory( shm );
}

void deleteSharedMemory( memory_t* mem )
{
    cleanup_memory( mem );
}

void runSimulation( double timeInSec )
{
    int shm = openMemory( SHM_NAME );
    memory_t* memory = loadMemory( shm );
    pid_t pidSimulation = getpid();
    pid_t citizenManagerPid = createCitizenManager();
    pid_t spyNetworkPid = createSpyNetwork();
    pid_t enemyCountryPid = createEnemyCountry();
    pid_t counterManagerPid = createCounterIntelligenceOfficerManager();
    pid_t monitorPid = createMonitor();
    pid_t timerPid = createTimer( pidSimulation, timeInSec );

    while ( runnable )
    {
        memory->memory_has_changed = true;
        pause();
        if ( ENABLE_GLOBAL_LOGS )
        {
            log_red( "SIGNAL SENT" );
        }
        kill( citizenManagerPid, SIGCONT );
        kill( spyNetworkPid, SIGCONT );
        kill( counterManagerPid, SIGCONT );
    }
    kill( spyNetworkPid, SIGTERM );
    kill( enemyCountryPid, SIGTERM );
    kill( citizenManagerPid, SIGTERM );
    kill( counterManagerPid, SIGTERM );

    waitpid( counterManagerPid, NULL, 0 );

    memory->memory_has_changed = true;

    unloadMemory( memory );
    closeMemory( shm );

    waitpid( timerPid, NULL, 0 );
    waitpid( citizenManagerPid, NULL, 0 );
    waitpid( spyNetworkPid, NULL, 0 );
    waitpid( enemyCountryPid, NULL, 0 );

    waitpid( monitorPid, NULL, 0 );
}