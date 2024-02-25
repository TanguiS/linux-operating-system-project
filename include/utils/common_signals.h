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
 * @file signal_citizen.h
 * @brief /!\ header, manage the citizen signal to process the turn /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __SIGNAL_CITIZEN_HH__
#define __SIGNAL_CITIZEN_HH__

void initHandlerContSignal();

void handlerContSignal( int signal );

void initHandlerEndSignal();

void handlerEndSignal( int signal );

#endif /* __SIGNAL_CITIZEN_HH__ */