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
 * @file manage_timer.h
 * @brief /!\ header, manage the timer in order to sync the simulation /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __MANAGE_TIMER_HH__
#define __MANAGE_TIMER_HH__

#include <sys/types.h>

/**
 * @brief execute the Timer
 * 
 * @param interval number of time between each turn
 * @param numberTurns the number of turn in the simulation
 * @param pidSimulation the PID of the process wich execute this process
 */
void runTimer( double interval, int numberTurns, pid_t pidSimulation );

/**
 * @brief Check the parameters & convert them
 * 
 * @param argc 
 * @param argv 
 * @param resultPidSimulation string from argv to pid_t
 * @param resultTime string from argv to int
 */
void handleTimerParameters( int argc, char** argv, pid_t* resultPidSimulation, double* resultTime );

#endif /* __MANAGE_TIMER_HH__ */