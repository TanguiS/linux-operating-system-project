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
 * @file action_citizen.h
 * @brief /!\ header, manage the citizen in the city /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __ACTION_CITIZEN_HH__
#define __ACTION_CITIZEN_HH__

#include "citizen_manager.h"

#define work_begin 8.             /**<-- Time in hour when the work begin */
#define work_end 17.              /**<-- Time in hour when the work end */
#define work_end_market_boyz 19.5 /**<-- Time in hour when the work end for the market worker */
#define market_begin 17.          /**<-- Time in hour when the market can be accessible */
#define market_end 19.5           /**<-- Time in hour when the market can no longer be accessible */

/**
 * @brief Thread function handler for Citizens
 * 
 * @param vargp 
 * @return void* 
 */
void* handleCitizensThreadCall( void* vargp );

#endif /* __ACTION_CITIZEN_HH__ */