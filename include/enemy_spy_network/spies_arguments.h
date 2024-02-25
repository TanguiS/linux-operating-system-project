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
 * @file spies_arguments.h
 * @brief /!\ all structs that a spy need /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#ifndef __ARG_HH__
#define __ARG_HH__

#include "./memory/memory.h"
#include "common/handle_fatal_error.h"

typedef enum
{
    AFTERNOON,
    NIGHT_FIRST_HALF,
    NIGHT_SECOND_HALF,
    MORNING_FIRST_HALF,
    MORNING_SECOND_HALF
} stealing_time;

typedef enum _day_objective
{
    HOME,
    MARKET,
    SPYING
} day_objective_t;

typedef enum _night_objectiv
{
    MAILBOX,
    STEAL,
    GO_BACK_HOME
} night_objectiv_t;

typedef struct _spies_pargs
{
    int id_spy;

    day_objective_t day_objective;
    night_objectiv_t night_objective;

    int turn_number_to_move;
    int turn_number_to_heal;

    bool list_of_companies_visited[NB_OF_COMPANY];
    company_t company_to_steal;
    point_t point_closed_company;

    int observation_turn_number;
    int stealing_turn_number;

    bool has_time_to_steal;
    stealing_time time_to_steal;
    bool can_i_steal;
    bool flag_to_steal_next_night;
    int turn_number_to_steal;
    bool want_to_steal;

    bool has_a_message;
    message_t message;

} spies_pargs;

#endif