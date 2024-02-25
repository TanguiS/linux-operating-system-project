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
 * @file manage_timer.c
 * @brief /!\ source, manage the timer in order to sync the simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "timer/manage_timer.h"
#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "memory/memory.h"
#include "timer/handle_timer_signal.h"
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define INIT_TIME ( 5. * 60. ) /**< Init Time if the simulation in min */

typedef struct sigaction sigaction_t; /**< shortcut */

static double _timeSpend = .0; /**< Time speed since the beginning of the simulation */
static double _interval = 0.;  /**< Time between eache turn */

/**
 * @brief send SIGTERM signal to the parent process
 * 
 * @param pidSimulation PID of the parent process
 */
static void sendEndSimulationSignal( pid_t pidSImulation )
{
    kill( pidSImulation, SIGTERM );
}

/**
 * @brief send SIGALARM to the parent process
 * 
 * @param pidSimulation PID of the parent process
 */
static void sendStartTurnSignal( pid_t pidSimulation )
{
    kill( pidSimulation, SIGALRM );
}

/**
 * @brief Allow the SIGALARM handler to increment the time
 * 
 */
static void incrementTimer()
{
    _timeSpend += _interval;
}

/**
 * @brief Set the time in hours of the current turn 
 * 
 * @return 
 */
static void setSimulationTime( memory_t* memory )
{
    incrementTimer();
    memory->simulationTime = fmod( ( ( _timeSpend / _interval ) * 10. + INIT_TIME ) / 60., 24. );
    memory->elapsed_time = _timeSpend;
}

/**
 * @brief 
 * 
 * @param memory 
 */
static void initSimulationTime( memory_t* memory )
{
    memory->simulationTime = ( INIT_TIME ) / 60.;
}

/**
 * @brief Run the timer
 * 
 * @param timeOfSimulation Duration of the simulation
 * @param pidSimulation PID of the parent process
 */
static void run( double timeOfSimulation, pid_t pidSimulation )
{
    const int shm = openMemory( SHM_NAME );
    memory_t* memory = loadMemory( shm );
    initSimulationTime( memory );
    if ( ENABLE_LOG_TIMER )
    {
        log_info( "Start time inside the simulation : %f", memory->simulationTime );
    }
    time_t start = time( NULL );
    while ( _timeSpend < timeOfSimulation )
    {
        sendStartTurnSignal( pidSimulation );
        usleep( _interval );
        setSimulationTime( memory );
    }
    if ( ENABLE_LOG_TIMER )
    {
        log_debug( "time elapsed : %f", difftime( time( NULL ), start ) );
        log_info( "time elapsed inside the simulation ! %f", memory->simulationTime );
    }
    sendEndSimulationSignal( pidSimulation );
    unloadMemory( memory );
    closeMemory( shm );
}

/**
 * @brief Set the Timer object
 * 
 * @param numberTurns Number of turn
 * @param pidSimulation 
 */
static void setTimer( int numberTurns, pid_t pidSimulation )
{
    double timeOfSimulation = (double)numberTurns * _interval;
    if ( ENABLE_LOG_TIMER )
    {
        log_debug( "time of simulation = %f\n", timeOfSimulation / 1000000 );
    }
    run( timeOfSimulation, pidSimulation );
}

/** PUBLIC FUNCTIONS */

void handleTimerParameters( int argc, char** argv, pid_t* resultPidSimulation, double* resultTimeInUS )
{
    if ( argc != 3 )
    {
        handleFatalError( "\n\nNot Enough argument : WARNING This program should not be executed by human, but by an other precessus\n\n" );
    }
    *resultPidSimulation = (int)strtol( argv[1], NULL, 10 );
    *resultTimeInUS = strtod( argv[2], NULL ) * 1000000;
}

void runTimer( double interval, int numberTurns, pid_t pidSimulation )
{
    _interval = interval;
    setTimer( numberTurns, pidSimulation );
}