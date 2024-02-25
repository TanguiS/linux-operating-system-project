#ifndef PATH_H
#define PATH_H

#include "memory/memory.h"

/**
 * @brief Fill the points param with the two other possible points
 * 
 * @param start the starting point
 * @param drow the row delta
 * @param dcol the column delta 
 * @param points the two other points to fill
 */
void getPossiblePoints( point_t start, int drow, int dcol, point_t points[2] );

/**
 * @brief Return the optimized next point to go to end from start with a breadth-first search.
 * 
 * @param start the starting point
 * @param end the ending point
 * @param map the map of the simulation
 * @return point_t 
 */
point_t getNextPoint( point_t start, point_t end, cell_t map[MAP_SIZE][MAP_SIZE] );

/**
 * @brief Check if the given point has valid coordinates
 * 
 * @param point the point to check
 * @return bool
 */
bool isValidePoints( point_t point );

/**
 * @brief Return the next point to go to end from start, with a 80% to go to the optimized one,
 * and 20% to go to an adjacent one.
 * 
 * @param start the starting point
 * @param end the ending point
 * @param map the map of the simulation
 * @param randBuffer the random number generator structure
 * @return point_t 
 */
point_t getRandomNextPointFromDrandGenerator( point_t start, point_t end, cell_t map[MAP_SIZE][MAP_SIZE], struct drand48_data* randBuffer );

/**
 * @brief get the closest not full supermarket from a given point
 * 
 * @param start the starting point
 * @param map the map to search the super market
 * @return point_t
 */
point_t getClosestSuperMarket( point_t start, cell_t map[MAP_SIZE][MAP_SIZE] );

/**
 * @brief update the map population
 * 
 * @param currentLocation the current location point
 * @param nextLocation the next location point
 * @param map the map to update the population
 */
void updatePopulation( point_t currentLocation, point_t nextLocation, cell_t map[MAP_SIZE][MAP_SIZE] );

#endif