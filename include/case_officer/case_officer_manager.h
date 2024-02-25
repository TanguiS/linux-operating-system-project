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
 * @file case_officer_manager.h
 * @brief /!\ header, manage the case officer in the city /!\
 * @author DUCASTEL Matéo <mateo.ducastel@ecole.ensicaen.fr>
 * @version 1.0.0
 * @date
 */

#ifndef __CASE_OFFICER_MANAGER_HH__
#define __CASE_OFFICER_MANAGER_HH__

#include "memory/memory.h"
#include <mqueue.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct thread_storage
{
    pthread_t case_officer;
    pthread_mutex_t accessMemory;
    pthread_mutex_t mutex;
    pthread_cond_t onTurnAction;
    struct drand48_data randBuffer;
} thread_storage_t;

typedef struct case_officer_thread
{
    bool altercation;
    double timeFirstVisitMailBox;
    double timeSecondVisitMailBox;
    bool isGoingToMailBox;
    double timeSuperMarket;
    bool isGoingToSuperMarket;
    bool firstPassageDone;
    bool secondPassageDone;
    double timeMessage;
    bool isSendingMessages;
    int injury;
    message_t mailBoxMessages[NB_OF_MSG_MAX];
    int nbOfMessages;
} case_officer_pargs_t;

void runCaseOfficerManager();

#endif /* __CASE_OFFICER_MANAGER_HH__ */