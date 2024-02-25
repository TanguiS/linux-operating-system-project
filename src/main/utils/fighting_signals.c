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
 * @file signal_citizen.c
 * @brief /!\ source, manage the citizen signal to process the turn /!\
 * @author SENG Thomas <thomas.seng@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "utils/fighting_signals.h"
#include "common/logger.h"
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct sigaction sigaction_t;

extern volatile bool runnable;

void initHandlerUsr2Signal()
{
    sigaction_t firegunShot;
    memset( &firegunShot, '\0', sizeof( firegunShot ) );
    firegunShot.sa_handler = &handlerUsr2Signal; /**< Function from parent process to deal the SIGTERM signal */
    sigaction( SIGUSR2, &firegunShot, NULL );
}

void handlerUsr2Signal( int signal, pid_t pid )
{
    if ( signal == SIGUSR2 )
    {
        kill( pid, signal );
    }
}