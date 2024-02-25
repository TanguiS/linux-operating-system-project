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
 * @file main_case_officer_manager.c
 * @brief /!\ exec, manage the case officer in the city /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "case_officer/case_officer_manager.h"
#include "utils/common_signals.h"

int main( void )
{
    initHandlerContSignal();
    initHandlerEndSignal();
    runCaseOfficerManager();
    exit( EXIT_SUCCESS );
}