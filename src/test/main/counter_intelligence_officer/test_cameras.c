#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include "counter_intelligence_officer/cameras.h"
#include "criterion/criterion.h"
#include "memory/memory.h"
#include "utils/other.h"
#include "utils/path.h"
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT 1
#define OUTPUT 0

volatile bool run = true;
volatile bool runnable = true;

int main( void )
{
    pid_t pid;
    int turn = 0;
    int reader;
    int suspiciousId[2];
    int tmp;
    char next;
    int flags;
    int shm = createMemory( SHM_NAME );
    memory_t* memory = loadMemory( shm );
    point_t closeCompany = { 1, 1 };
    point_t randomLocation = { 3, 6 };

    memory->simulationTime = 8.;
    citizen_t* test = (citizen_t*)getCharacterFromId( ( NB_OF_SPIES + NB_CASE_OFFICER + NB_OF_COUNTER + 10 ), &tmp, &memory->characters );

    if ( pipe( suspiciousId ) == -1 )
    {
        perror( "[ERROR pipe]" );
        exit( EXIT_FAILURE );
    }
    pid = fork();

    if ( pid == -1 )
    {
        perror( "[ERROR pid]" );
        exit( EXIT_FAILURE );
    }
    if ( pid > 0 )
    {
        flags = fcntl( suspiciousId[OUTPUT], F_GETFL, 0 );
        if ( flags == -1 )
        {
            handleFatalError( "perror[FLASG]" );
        }
        if ( fcntl( suspiciousId[OUTPUT], F_SETFL, flags | O_NONBLOCK ) == -1 )
        {
            handleFatalError( "perror[NONBLOCK fcntl]" );
        }
        close( suspiciousId[INPUT] );
        log_green( "INIT position citizen test : (%d, %d)\n", test->location.row, test->location.column );
        kill( pid, SIGUSR1 );
        while ( turn < 10000 )
        {
            log_green( "Wait for next turn :\nt: increment time of an hour\nn: go to nigth time\nd: go to day time\ne: go close to a company\nh: go to home\nm: go to (3,6)\nq: quit\n" );
            fflush( stdin );
            next = getchar();
            fflush( stdin );
            switch ( next )
            {
            case 't':
                log_yellow( "Increment" );
                memory->simulationTime += 0.1666666666666666666666666666666666667;
                break;
            case 'n':
                log_yellow( "Night" );
                memory->simulationTime = 4.;
                break;
            case 'd':
                log_yellow( "Day" );
                memory->simulationTime = 14.;
                break;
            case 'e':
                log_yellow( "Close Company" );
                updatePopulation( test->location, closeCompany, memory->map.cells );
                test->location = closeCompany;
                break;
            case 'm':
                log_yellow( "(3, 6)" );
                updatePopulation( test->location, randomLocation, memory->map.cells );
                test->location = randomLocation;
                break;
            case 'h':
                log_yellow( "Home" );
                updatePopulation( test->location, test->home, memory->map.cells );
                test->location = test->home;
                break;
            case 'q':
                turn = 10000 + 10;
                kill( pid, SIGUSR2 );
                break;
            }
            log_magenta( "  Location citizen : (%d, %d),\n  Time simulation : %f\n", test->location.row, test->location.column, memory->simulationTime );
            kill( pid, SIGUSR1 );
            sleep( 1 );
            while ( read( suspiciousId[OUTPUT], &reader, sizeof( reader ) ) > 0 )
            {
                log_red( "Cette if est suspect : %d, !!!!!\n", reader );
            }
            turn++;
        }
        runnable = false;
        run = false;
        kill( pid, SIGUSR2 );
        close( suspiciousId[OUTPUT] );
        log_info( "END gestion with runnable at value : %d", runnable );
        wait( NULL );
    }
    else if ( pid == 0 )
    {
        runCameras( suspiciousId );
    }

    cleanup_memory( memory );
    unloadMemory( memory );
    closeMemory( shm );

    exit( EXIT_SUCCESS );
    return EXIT_SUCCESS;
}