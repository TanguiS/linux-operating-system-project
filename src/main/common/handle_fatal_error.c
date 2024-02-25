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
 * @file handle_fatal_error.c
 * @brief /!\ source, manage a fatal error and exit program /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#include "common/handle_fatal_error.h"
#include "common/logger.h"
#include <stdlib.h>

void handleFatalError( const char* err )
{
    log_error( err );
    exit( EXIT_SUCCESS );
}