#ifndef MEMORY_H
#define MEMORY_H

#include "data.h"
#include <stdbool.h>
#include <stdlib.h>

struct monitor_s
{
    int has_to_update;
};

typedef struct point
{
    int row;
    int column;
} point_t;

typedef struct cell
{
    int type;
    point_t coords;
    int maxPop;
    int currentPop;
} cell_t;

typedef struct company
{
    point_t cell;
    int nbOfEmployee;
    int information[NB_OF_INFORMATION_FIELDS];
} company_t;

typedef struct message
{
    char content[MAX_MESSAGE_LENGTH];
    int priority;
} message_t;

typedef struct citizen
{
    point_t home;
    point_t location;
    point_t working_place;
    int id;
    state_e currentState;
} citizen_t;

typedef struct spy
{
    int id;
    int health_points;
    point_t location;
    point_t home;
    int nb_of_stolen_companies;
    message_t stolen_message;
    bool has_license_to_kill;
    bool health_points_has_changed;
} spy_t;

typedef struct case_officier
{
    int id;
    int health_points;
    point_t location;
    point_t home;
    point_t mailbox;
} case_officer_t;

typedef struct couterintelligence_officer
{
    int id;
    int health_points;
    point_t location;
    point_t city_hall;
    point_t mailbox_location;
    pid_t myPid;
    pid_t targetPid;
    int targeted_character_id;
    int whoIsHere;
    bool firstShoot;
    bool didIHitTarget;
    bool canFightBegin;
    bool fightInProgress;
    bool doubleDamage;
    int nb_of_messages;
    message_t copiedMessages[NB_OF_MSG_MAX];
} counterintelligence_officer_t;

typedef struct characters
{
    citizen_t citizens[NB_OF_CITIZENS];
    int citizens_state[NUMBER_STATE];
    spy_t spies[NB_OF_SPIES];
    case_officer_t case_officers[NB_CASE_OFFICER];
    counterintelligence_officer_t counter_officers[NB_OF_COUNTER];
} characters_t;

typedef struct map
{
    int columns;
    int rows;
    cell_t cells[MAP_SIZE][MAP_SIZE];
} map_t;

typedef struct memory
{
    int shmFd;
    int memory_has_changed;
    map_t map;
    double simulationTime;
    double elapsed_time;
    int simulation_has_ended;
    characters_t characters;
    int mailbox_nb_of_msgs;
    message_t mailbox_content[NB_OF_MSG_MAX];
    company_t companies[NB_OF_COMPANY];
} memory_t;

/**
 * @brief Create a Memory object
 *
 * @param name
 * @return int
 */
int createMemory( const char* name );

/**
 * @brief
 *
 * @param mem
 */
void cleanup_memory( memory_t* mem );

/**
 * @brief open
 *
 * @param name
 * @return int
 */
int openMemory( const char* name );

/**
 * @brief close memory
 *
 * @param shm
 * @param name
 */
void closeMemory( const int shm );

/**
 * @brief load
 *
 * @param shm
 * @return memory_t*
 */
memory_t* loadMemory( const int shm );

/**
 * @brief unload
 *
 * @param mem
 */
void unloadMemory( memory_t* mem );

#endif