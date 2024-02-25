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
 * @file create_citizen.h
 * @brief /!\ header, data, regroup all the datas  whose can be shared /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __DATA_HH__
#define __DATA_HH__

#define SHM_SIZE 100000 // TODO find correct value

#define SHM_NAME "/spy"

#define MAP_SIZE 7

#define NB_OF_RESIDENTIAL 11
#define NB_OF_COMPANY 8

enum cell_type_e
{
    WASTELAND,            /*!< An empty cell. */
    RESIDENTIAL_BUILDING, /*!< A cell that contains a residential building. */
    CITY_HALL,            /*!< A cell that contains the City Hall. */
    COMPANY,              /*!< A cell that contains a company. */
    SUPERMARKET           /*!< A cell that contains a supermarket. */
};

#define BIG_COMPANY_MIN_EMPLOYEE 30
#define MEDIUM_COMPANY_MIN_EMPLOYEE 20
#define SMALL_COMPANY_MIN_EMPLOYEE 10

#define NB_OF_INFORMATION_FIELDS 8
#define CRUCIAL_INFORMATION 4
#define STRONG_INFORMATION 3
#define MEDIUM_INFORMATION 2
#define LOW_INFORMATION 1
#define VERY_LOW_INFORMATION 0

#define RESIDENTIAL_POP 15
#define WASTELAND_POP __INT_MAX__
#define COMPANY_POP 50
#define SUPERMARKET_POP 30
#define CITY_HALL_POP 20

#define NB_OF_CITIZENS 127
#define NB_OF_SPIES 3
#define NB_CASE_OFFICER 1
#define NB_OF_COUNTER 1

#define NB_OF_CITY_HALL_EMPLOYEE 10
#define NB_OF_EMPLOYEE_MIN_COMPANY 5
#define NB_OF_SUPERMARKET 2
#define NB_OF_SUPERMARKET_EMPLOYEE 3

#define MAX_DIST_TO_MAILBOX 4

#define EMPTY ""
#define NB_OF_MSG_MAX 3
#define FAKE_MESSAGE "SALOOOOOOOOOOOOOOO"
#define MAX_MESSAGE_LENGTH 100

#define NO_TARGET -1
#define STARTING_HP 10

#define SPY_ID 0
#define CASE_OF_ID 1
#define COUNTER_ID 2
#define CITIZEN_ID 3

#define NUMBER_STATE 3

typedef enum
{
    INDEX_HOME = 0,
    INDEX_WORK = 1,
    INDEX_MOVING = 2
} state_e;

#endif /* __DATA_HH__ */