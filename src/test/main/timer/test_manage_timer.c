/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

/**
 * @file test_manage_timer.c
 * @brief /!\ criterion unit test, test the timer /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include <stdio.h>

#include "criterion/criterion.h"
#include "timer/manage_timer.h"

void setup( void )
{
    puts( "Runs before the test" );
}

void teardown( void )
{
    puts( "Runs after the test" );
}

Test( test_manage_timer, sample, .init = setup, .fini = teardown )
{
    // test contents
    puts( "Test_manage_timer is running...." );
    fflush( stdout );
}

/*
Test(suite_name, test_name, .init = setup, .fini = teardown) {
    // test contents
}
*/