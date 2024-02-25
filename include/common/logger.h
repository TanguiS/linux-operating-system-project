/*
 * Author: Babacar (https://stackoverflow.com/users/3599371)
 */

#ifndef LOGGER_H
#define LOGGER_H

/**
 * \file logger.h
 *
 * Defines some useful logging functions.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"
#define BRIGHT_MAGENTA "\x1B[95m"

#define ENABLE_GLOBAL_LOGS 0
#define ENABLE_LOG_TIMER 0
#define ENABLE_LOG_CITIZENS 0
#define ENABLE_LOG_CASE_OFFICER 0
#define ENABLE_LOG_SPY 0
#define ENABLE_LOG_ENEMY_COUNTRY 0
#define ENABLE_LOG_COUNTER_INTELLIGENCE_OFFICER 0

void log_error( const char* message, ... );

void log_info( const char* message, ... );

void log_debug( const char* message, ... );

void log_yellow( const char* message, ... );

void log_green( const char* message, ... );

void log_magenta( const char* message, ... );

void log_red( const char* message, ... );

void log_bright_magenta( const char* message, ... );

#endif /* LOGGER_H */
