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
 * @file create_case_officer.h
 * @brief /!\ header, manage the creation of the case officer in the city /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __CREATE_CASE_OFFICER_HH__
#define __CREATE_CASE_OFFICER_HH__

#include "case_officer/case_officer_manager.h"

/**
 * @brief Create a Case officer thread object
 * 
 * @param tid 
 * @param pargs 
 */
void createCaseOfficerThread( pthread_t* tid, case_officer_pargs_t* pargs );

/**
 * @brief Properly delete the Thread
 * 
 */
void deleteCaseOfficerThreadStorage();

#endif /* __CREATE_CASE_OFFICER_HH__ */