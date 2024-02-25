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
 * @file cameras.h
 * @brief /!\ header, manage the different cameras' action to inform the counter intelligence officer the suspicious citizen /!\
 * @author STEIMETZ Tangui <steimetz.tangui@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __CAMERAS_HH__
#define __CAMERAS_HH__

#include <ctype.h>

/**
 * @brief Signals for Cameras
 * 
 * @param signal 
 */
void signalCamerasHandler( int signal );

/**
 * @brief Run the Cameras
 * 
 * @param idSuspiciousCharactersPipe 
 */
void runCameras( int idSuspiciousCharactersPipe[] );

#endif /* __CAMERAS_HH__ */