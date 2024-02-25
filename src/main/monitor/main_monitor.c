/*
 * The License to Kill Project
 *
 * Copyright (C) 2021 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory/memory.h"
#include "monitor/monitor.h"
#include "monitor/monitor_common.h"

extern WINDOW* main_window;
extern int old_cursor;

/**
 * \file main.c
 *
 * \brief Creates a new "Terminal User Interface" using ncurses and associated
 * with the given shared memory.
 *
 * The size of the terminal must have at least 40 rows and 145 columns. It
 * is decomposed in four sub-windows:
 * - The "city map and status" window (upper-left)
 * - The "character information" window (upper-right)
 * - The "mailbox content" window (bottom-left)
 * - The "enemy country monitor" window (bottom-right)
 * 'Q', 'q' and 'Esc' keys are used to exit from the TUI.
 */
int main( void )
{
    int rows;
    int cols;
    int key;
    bool runnable = true;
    int shm = openMemory( SHM_NAME );
    memory_t* memory = loadMemory( shm );
    monitor_t* monitor;

    monitor = (monitor_t*)malloc( sizeof( monitor_t ) );
    monitor->has_to_update = 0;

    set_timer();
    set_signals();

    if ( ( main_window = initscr() ) == NULL )
    {
        quit_after_error( "Error initializing library ncurses!" );
    }

    clear();                    /* Start from an empty screen. */
    cbreak();                   /* No line buffering, pass every key pressed. */
    noecho();                   /* Do not echo the keyboard input. */
    old_cursor = curs_set( 0 ); /* Use and invisible cursor. */
    keypad( stdscr, TRUE );     /* Allows functions keys, arrows, etc. */

    start_color();        /* Allow using colors... */
    create_color_pairs(); /* ... and create color pairs to use */

    if ( !is_terminal_size_larger_enough( &rows, &cols ) )
    {
        quit_after_error( "Minimal terminal dimensions: 45 rows and 140 columns!" );
    }

    /* Initialize terminal user interface elements */
    init_monitor_elements( main_window, memory, rows, cols );

    /*  Loop and get user input  */
    while ( runnable )
    {
        key = getch();

        switch ( key )
        {
        case 'Q':
            runnable = false;
            break;
        case 'q':
            runnable = false;
            break;
        case 27:
            quit_nicely( NO_PARTICULAR_REASON );
            break;
        default:
            break;
        }
        if ( memory->memory_has_changed )
        {
            update_values( memory );
            memory->memory_has_changed = 0;
        }
    }
    quit_nicely( NO_PARTICULAR_REASON );
    endwin();
    free( monitor );
    unloadMemory( memory );
    closeMemory( shm );
}