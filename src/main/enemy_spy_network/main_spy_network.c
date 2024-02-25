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
 * @file main_citizen_manager.c
 * @brief /!\ handle all the spy network /!\
 * @author Picque Kylian <kylian.picque@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */
#include "./enemy_spy_network/create_spy_network.h"
#include "./enemy_spy_network/fight_actions.h"
#include "utils/common_signals.h"

int main()
{
    initHandlerContSignal();
    initHandlerEndSignal();

    initHandlerUsr1Signal();
    initHandlerUsr2Signal();
    runSpiesManager();

    exit( EXIT_SUCCESS );
}