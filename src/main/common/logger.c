/*
 * Author: Babacar (https://stackoverflow.com/users/3599371)
 */

#include "common/logger.h"

void log_format( const char* tag, const char* message, va_list args )
{
    time_t now;
    time( &now );
    char* date = ctime( &now );
    date[strlen( date ) - 1] = '\0';
    printf( "%s [%s] ", date, tag );
    vprintf( message, args );
    printf( "\n" );
}

void log_error( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( RED );
    log_format( "Error", message, args );
    printf( RESET "" );
    va_end( args );
}

void log_info( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    log_format( "Info", message, args );
    va_end( args );
}

void log_debug( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( CYN );
    log_format( "Debug", message, args );
    printf( RESET "" );
    va_end( args );
}

void log_yellow( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( YEL );
    log_format( "Info", message, args );
    printf( RESET "" );
    va_end( args );
}

void log_green( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( GRN );
    log_format( "Info", message, args );
    printf( RESET "" );
    va_end( args );
}

void log_magenta( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( MAG );
    log_format( "Info", message, args );
    printf( RESET "" );
    va_end( args );
}

void log_red( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( RED );
    log_format( "Info", message, args );
    printf( RESET "" );
    va_end( args );
}

void log_bright_magenta( const char* message, ... )
{
    va_list args;
    va_start( args, message );
    printf( BRIGHT_MAGENTA );
    log_format( "Info", message, args );
    printf( RESET "" );
    va_end( args );
}