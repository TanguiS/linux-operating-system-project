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
 * @file main_enemy_country.c
 * @brief /!\ exec, manage the enemy country /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "enemy_country/enemy_country_manager.h"
#include "utils/common_signals.h"
#include <stdlib.h>

int main( void )
{
    initHandlerEndSignal();
    openMQ();
    exit( EXIT_SUCCESS );
}