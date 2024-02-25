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
 * @file action_case_officer.h
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __ACTION_CASE_OFFICER_HH__
#define __ACTION_CASE_OFFICER_HH__

#include "case_officer/case_officer_manager.h"

#define TIME_UNDEFINED -1.
#define START_DAY_TIME 8.
#define END_DAY_TIME 17.
#define START_SUPERMARKET_TIME 17.
#define END_SUPERMARKET_TIME 19.
#define START_MESSAGE_TIME 22.
#define END_MESSAGE_TIME 0.

#define NO_INJURY -1
#define SMALL_INJURY 1
#define BIG_INJURY 2

/**
 * @brief Thread function handler for Case officer
 * 
 * @param vargp 
 * @return void* 
 */
void* handleCaseOfficerThreadCall( void* vargp );

#endif /* __ACTION_CASE_OFFICER_HH__ */