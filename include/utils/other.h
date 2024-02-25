#ifndef OTHER_H
#define OTHER_H

#include "memory/memory.h"

#define ERROR -1
#define NUMBER_TURNS_IN_HOUR 6
#define TIME_TURN 1 / NUMBER_TURNS_IN_HOUR
#define LIMIT 1E-5

/**
 * @brief Check whether two cells are the same or not
 * 
 * @param cell1 the first cell
 * @param cell2 the second cell
 * 
 * @return bool
 */
bool compare_cells( cell_t* cell1, cell_t* cell2 );

/**
 * @brief Check whether two points are the same or not
 * 
 * @param firstPoint the first point
 * @param secondPoint the second point
 * 
 * @return bool
 */
bool compare_points( point_t firstPoint, point_t secondPoint );

/**
 * @brief Get the cell in the map from a given point
 * 
 * @param point the point to get the cell from
 * @param map the map from the shared memory
 * 
 * @return cell_t*
 */
cell_t* getCellFromPoint( point_t point, cell_t map[MAP_SIZE][MAP_SIZE] );

/**
 * @brief Get a character from a given id
 * 
 * @param id the id of the character
 * @param typeCharacter the type of the character to fill (SPY, CITIZEN, etc..)
 * @param characters the list of the characters in the simulation
 * 
 * @return void* a pointer to the character found
 */
void* getCharacterFromId( int id, int* typeCharacter, characters_t* characters );

/**
 * @brief Get the location from the character with a given id
 * 
 * @param id the id of the character
 * @param characters the list of the characters in the simulation
 * 
 * @return point_t*
 */
point_t* getPointFromId( int id, characters_t* characters );

/**
 * @brief Get the cell in the map from a given point
 * 
 * @param point the point to get the cell from
 * @param map the map from the shared memory
 * 
 * @return cell_t*
 */
cell_t* getCellFromId( int id, characters_t* characters, cell_t map[MAP_SIZE][MAP_SIZE] );

/**
 * @brief Get a random time between the start and the end times
 * 
 * @param start the starting time
 * @param end the ending time
 * @param randBuffer the random number generator structure
 * 
 * @return double
 */
double getRandomTimeBetween( double start, double end, struct drand48_data* randBuffer );

#endif