#include "common/logger.h"
#include <criterion/criterion.h>
#include <stdio.h>

void setup( void )
{
    puts( "Runs common/logger.c testing : " );
}

void teardown( void )
{
    puts( "Runs after the test" );
}

Test( test_logger, sample, .init = setup, .fini = teardown )
{
    // test contents
    puts( "Test 1 is running...." );
    fflush( stdout );
}

Test( test_logger, sample2, .init = setup, .fini = teardown )
{
    // test contents
    puts( "Test 2 is running...." );
    fflush( stdout );
}

/*
Test(suite_name, test_name, .init = setup, .fini = teardown) {
    // test contents
}
*/