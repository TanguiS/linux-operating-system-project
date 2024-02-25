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
 * @file main_enemy_country.c
 * @brief /!\ source, manage the enemy country /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "enemy_country/enemy_country_manager.h"
#include "common/logger.h"
#include "data.h"
#include <mqueue.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile bool runnable = 1;

void openMQ()
{
    int n = -1;
    unsigned int priority;
    mqd_t mq = -1;
    struct mq_attr attr;
    char buffer[MAX_MESSAGE_LENGTH];
    memset( buffer, '\0', MAX_MESSAGE_LENGTH );
    FILE* fptr;
    while ( mq == -1 )
    {
        mq = mq_open( MESSAGE_QUEUE_NAME, O_RDONLY, 0666, NULL );
    }
    fptr = fopen( "messages_received.txt", "w" ); // empty the file
    fclose( fptr );

    if ( ENABLE_LOG_ENEMY_COUNTRY )
    {
        log_green( "WAITING FOR MESSAGES" );
    }
    while ( runnable )
    {
        mq_getattr( mq, &attr );
        while ( attr.mq_curmsgs > 0 )
        {
            n = mq_receive( mq, buffer, attr.mq_msgsize, &priority );

            if ( ENABLE_LOG_ENEMY_COUNTRY && n == -1 )
            {
                log_red( "ERROR WHILE READING MESSAGE !" );
            }
            mq_getattr( mq, &attr );
            if ( n != -1 && strcmp( FAKE_MESSAGE, buffer ) )
            {
                fptr = fopen( "messages_received.txt", "a" );
                if ( fptr == NULL )
                {
                    if ( ENABLE_LOG_ENEMY_COUNTRY )
                    {
                        log_green( "ERROR opening messages_received.txt" );
                    }
                    exit( EXIT_FAILURE );
                }
                if ( ENABLE_LOG_ENEMY_COUNTRY )
                {
                    log_green( "MESSAGE RECEIVED %s", buffer );
                }
                fprintf( fptr, "[%d], %s\n", priority, buffer );
                fclose( fptr );
            }
        }
    }
    if ( ENABLE_LOG_ENEMY_COUNTRY )
    {
        log_red( "END OF PROG" );
    }
    mq_close( mq );
    mq_unlink( MESSAGE_QUEUE_NAME );
    exit( EXIT_SUCCESS );
}