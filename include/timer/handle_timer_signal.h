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
 * @file handle_timer_signal.h
 * @brief /!\ header, manage the sync with the timer /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __HANDLE_TIMER_SIGNAL_HH__
#define __HANDLE_TIMER_SIGNAL_HH__

#include <sys/types.h>
/**
 * @brief Init the SIGTERM signal
 * 
 */
void initHandlerEndSimulation();

/**
 * @brief Handle SIGALARM signal sended by the timer
 * 
 * @param signal 
 */
void handleTimerAlarm( int signal );

/**
 * @brief Handle SIGTERM signal sended by the timer
 * 
 * @param signal 
 */
void handleEndSimulationSignal( int signal );

/**
 * @brief Init the SIGALARM signal
 * 
 */
void initHandlerAlarmTurn();

/**
 * @brief Create and a Timer and run it
 * 
 * @param pidSimulation PID of parent process
 * @param timeInSec Time between each turn
 */
void createAndRunTimer( pid_t pidSimulation, double timeInSec );

#endif /* __HANDLE_TIMER_SIGNAL_HH__ */