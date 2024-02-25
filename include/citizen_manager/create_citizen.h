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
 * @brief /!\ header, manage the creation of the citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __CREATE_CITIZEN_HH__
#define __CREATE_CITIZEN_HH__

#include "citizen_manager.h"

/**
 * @brief Create a Citizens Threads object
 * 
 * @param tid 
 * @param pargs 
 */
void createCitizensThreads( pthread_t* tid, citizen_pargs_t* pargs );

/**
 * @brief Properly delete the Thread
 * 
 */
void deleteCitizensThreadStorage();

#endif /* __CREATE_CITIZEN_HH__ */