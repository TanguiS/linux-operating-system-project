#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "memory/memory.h"

int mapArray[MAP_SIZE][MAP_SIZE] = {
    {
        WASTELAND,
        COMPANY,
        WASTELAND,
        COMPANY,
        WASTELAND,
        WASTELAND,
        COMPANY,
    },
    {
        RESIDENTIAL_BUILDING,
        WASTELAND,
        SUPERMARKET,
        WASTELAND,
        WASTELAND,
        RESIDENTIAL_BUILDING,
        WASTELAND,
    },
    {
        WASTELAND,
        RESIDENTIAL_BUILDING,
        WASTELAND,
        RESIDENTIAL_BUILDING,
        WASTELAND,
        WASTELAND,
        RESIDENTIAL_BUILDING,
    },
    {
        WASTELAND,
        WASTELAND,
        WASTELAND,
        CITY_HALL,
        COMPANY,
        WASTELAND,
        WASTELAND,
    },
    {
        WASTELAND,
        RESIDENTIAL_BUILDING,
        COMPANY,
        SUPERMARKET,
        WASTELAND,
        WASTELAND,
        RESIDENTIAL_BUILDING,
    },
    {
        WASTELAND,
        WASTELAND,
        COMPANY,
        WASTELAND,
        RESIDENTIAL_BUILDING,
        WASTELAND,
        COMPANY,
    },
    {
        RESIDENTIAL_BUILDING,
        COMPANY,
        WASTELAND,
        WASTELAND,
        RESIDENTIAL_BUILDING,
        WASTELAND,
        RESIDENTIAL_BUILDING,
    },
};

memory_t* loadMemory( const int shm )
{
    return (memory_t*)mmap( NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0 );
}

void unloadMemory( memory_t* mem )
{
    if ( munmap( mem, SHM_SIZE ) == -1 )
    {
        printf( "Error [unmap()]: " );
        exit( EXIT_FAILURE );
    }
}

int hammingDistance( point_t firstCell, point_t secondCell )
{
    return abs( firstCell.column - secondCell.column ) + abs( firstCell.row - secondCell.row );
}

void findResidentialSpies( spy_t spies[NB_OF_SPIES], point_t residentialArray[NB_OF_RESIDENTIAL], point_t mailBox, map_t* map )
{
    int i, cpt = 0;
    for ( i = 0; i < NB_OF_SPIES; i++ )
    {
        for ( ; cpt < NB_OF_RESIDENTIAL; cpt++ )
        {
            int distance = hammingDistance( residentialArray[cpt], mailBox );
            if ( distance <= MAX_DIST_TO_MAILBOX && distance > 0 )
            {
                spies[i].home = residentialArray[cpt];
                spies[i].location = spies[i].home;
                map->cells[spies[i].location.row][spies[i].location.column].currentPop++;
                cpt++;
                break;
            }
        }
    }
}

void setWorkingPlaceCitizen( citizen_t* citizen, int index, company_t companyArray[NB_OF_COMPANY], point_t cityHall, point_t superMarketArray[NB_OF_SUPERMARKET] )
{
    int i, randomNumber;
    if ( index < NB_OF_CITY_HALL_EMPLOYEE )
    {
        citizen->working_place = cityHall;
        return;
    }
    for ( i = 0; i < NB_OF_SUPERMARKET; i++ )
    {
        if ( index < NB_OF_CITY_HALL_EMPLOYEE + ( i + 1 ) * NB_OF_SUPERMARKET_EMPLOYEE )
        {
            citizen->working_place = superMarketArray[i];
            return;
        }
    }
    for ( i = 0; i < NB_OF_COMPANY; i++ )
    {
        if ( index < NB_OF_CITY_HALL_EMPLOYEE + NB_OF_SUPERMARKET * NB_OF_SUPERMARKET_EMPLOYEE + ( i + 1 ) * NB_OF_EMPLOYEE_MIN_COMPANY )
        {
            citizen->working_place = companyArray[i].cell;
            companyArray[i].nbOfEmployee++;
            return;
        }
    }
    randomNumber = rand() % NB_OF_COMPANY;
    citizen->working_place = companyArray[randomNumber].cell;
    companyArray[randomNumber].nbOfEmployee++;
}

void setCompaniesInformationField( company_t companyArray[NB_OF_COMPANY] )
{
    int i;
    for ( i = 0; i < NB_OF_COMPANY; i++ )
    {
        companyArray[i].information[CRUCIAL_INFORMATION] = companyArray[i].nbOfEmployee > BIG_COMPANY_MIN_EMPLOYEE ? 2 : 0;
        companyArray[i].information[STRONG_INFORMATION] = companyArray[i].nbOfEmployee > BIG_COMPANY_MIN_EMPLOYEE ? 5 : 1;
        companyArray[i].information[MEDIUM_INFORMATION] = companyArray[i].nbOfEmployee > MEDIUM_COMPANY_MIN_EMPLOYEE ? 12 : 7;
        companyArray[i].information[LOW_INFORMATION] = companyArray[i].nbOfEmployee > SMALL_COMPANY_MIN_EMPLOYEE ? 20 : 11;
        companyArray[i].information[VERY_LOW_INFORMATION] = companyArray[i].nbOfEmployee > SMALL_COMPANY_MIN_EMPLOYEE ? 0 : 17;
    }
}

void initSpies( spy_t* spies )
{
    int i;
    for ( i = 0; i < NB_OF_SPIES; i++ )
    {
        spies[i].id = i;
        spies[i].has_license_to_kill = i == 0;
        spies[i].health_points = STARTING_HP;
        spies[i].nb_of_stolen_companies = 0;
        spies[i].health_points_has_changed = false;
    }
}

void initCounterOfficers( counterintelligence_officer_t* counter_officers, point_t cityHall, map_t* map )
{
    int i;
    for ( i = 0; i < NB_OF_COUNTER; i++ )
    {
        counter_officers[i].id = NB_OF_SPIES + NB_CASE_OFFICER + i;
        counter_officers[i].health_points = STARTING_HP;
        counter_officers[i].city_hall = cityHall;
        counter_officers[i].mailbox_location.row = -1;
        counter_officers[i].mailbox_location.column = -1;
        counter_officers[i].location = cityHall;
        counter_officers[i].targeted_character_id = NO_TARGET;
        counter_officers[i].whoIsHere = NO_TARGET;
        counter_officers[i].firstShoot = true;
        counter_officers[i].didIHitTarget = false;
        counter_officers[i].canFightBegin = false;
        counter_officers[i].fightInProgress = false;
        counter_officers[i].doubleDamage = false;
        counter_officers[i].nb_of_messages = 0;
        map->cells[counter_officers[i].location.row][counter_officers[i].location.column].currentPop++;
    }
}

void initCaseOfficers( case_officer_t* case_officers, point_t residentialArray[NB_OF_RESIDENTIAL], map_t* map )
{
    int i;
    case_officers[0].mailbox = residentialArray[rand() % NB_OF_RESIDENTIAL];
    for ( i = 0; i < NB_CASE_OFFICER; i++ )
    {
        case_officers[i].health_points = STARTING_HP;
        case_officers[i].home = residentialArray[rand() % NB_OF_RESIDENTIAL];
        case_officers[i].id = NB_OF_SPIES + i;
        case_officers[i].location = case_officers[i].home;
        case_officers[i].mailbox = case_officers[0].mailbox;
        map->cells[case_officers[i].location.row][case_officers[i].location.column].currentPop++;
    }
}

void initMessages( message_t* mailbox_content )
{
    int i;
    for ( i = 0; i < NB_OF_MSG_MAX; i++ )
    {
        strcpy( mailbox_content[i].content, EMPTY );
        mailbox_content[i].priority = -1;
    }
    strcpy( mailbox_content[0].content, "HEY GUYS" );
    mailbox_content[0].priority = 1;
}

void initCitizens( citizen_t citizens[NB_OF_CITIZENS], point_t residentialArray[NB_OF_RESIDENTIAL], point_t cityHall, point_t supermarketArray[NB_OF_SUPERMARKET], company_t companies[NB_OF_COMPANY], map_t* map )
{
    int i, currentResidential = 0;
    for ( i = 0; i < NB_OF_CITIZENS; i++ )
    {
        citizens[i].id = NB_OF_SPIES + NB_CASE_OFFICER + NB_OF_COUNTER + i;
        citizens[i].home = residentialArray[currentResidential];
        setWorkingPlaceCitizen( &citizens[i], i, companies, cityHall, supermarketArray );
        citizens[i].location = citizens[i].home;
        citizens[i].currentState = INDEX_HOME;
        map->cells[citizens[i].location.row][citizens[i].location.column].currentPop++;
        currentResidential = ( currentResidential + 1 ) % NB_OF_RESIDENTIAL;
    }
}

void initMap( map_t* map, point_t residentialArray[NB_OF_RESIDENTIAL], point_t* cityHall, point_t supermarketArray[NB_OF_SUPERMARKET], company_t companies[NB_OF_COMPANY] )
{
    int i, j;
    int k = 0, l = 0, m = 0;
    map->columns = MAP_SIZE;
    map->rows = MAP_SIZE;

    for ( i = 0; i < MAP_SIZE; i++ )
    {
        for ( j = 0; j < MAP_SIZE; j++ )
        {
            map->cells[i][j].coords.row = i;
            map->cells[i][j].coords.column = j;
            map->cells[i][j].type = mapArray[i][j];
            map->cells[i][j].currentPop = 0;
            if ( mapArray[i][j] == RESIDENTIAL_BUILDING )
            {
                residentialArray[k].row = i;
                residentialArray[k++].column = j;
                map->cells[i][j].maxPop = RESIDENTIAL_POP;
            }
            else if ( mapArray[i][j] == COMPANY )
            {
                companies[l].nbOfEmployee = 0;
                companies[l].cell.row = i;
                companies[l++].cell.column = j;
                map->cells[i][j].maxPop = COMPANY_POP;
            }
            else if ( mapArray[i][j] == SUPERMARKET )
            {
                supermarketArray[m].row = i;
                supermarketArray[m++].column = j;
                map->cells[i][j].maxPop = SUPERMARKET_POP;
            }
            else if ( mapArray[i][j] == CITY_HALL )
            {
                cityHall[0].row = i;
                cityHall[0].column = j;
                map->cells[i][j].maxPop = CITY_HALL_POP;
            }
            else
            {
                map->cells[i][j].maxPop = WASTELAND_POP;
            }
        }
    }
}

void initStateCharacters( characters_t* characters )
{
    characters->citizens_state[INDEX_HOME] = NB_OF_CITIZENS;
    characters->citizens_state[INDEX_WORK] = 0;
    characters->citizens_state[INDEX_MOVING] = 0;
}

void initMemory( memory_t* mem )
{
    point_t residentialArray[NB_OF_RESIDENTIAL];
    point_t supermarketArray[NB_OF_SUPERMARKET];
    point_t cityHall;
    mem->mailbox_nb_of_msgs = 1;
    mem->simulation_has_ended = 0;
    mem->simulationTime = 0.;
    mem->elapsed_time = 0.;

    initMap( &mem->map, residentialArray, &cityHall, supermarketArray, mem->companies );
    initStateCharacters( &mem->characters );
    initCitizens( mem->characters.citizens, residentialArray, cityHall, supermarketArray, mem->companies, &mem->map );
    initCaseOfficers( mem->characters.case_officers, residentialArray, &mem->map );
    initCounterOfficers( mem->characters.counter_officers, cityHall, &mem->map );
    initSpies( mem->characters.spies );
    initMessages( mem->mailbox_content );
    findResidentialSpies( mem->characters.spies, residentialArray, mem->characters.case_officers[0].mailbox, &mem->map );
    setCompaniesInformationField( mem->companies );
}

int openMemory( const char* name )
{
    return shm_open( name, O_RDWR, 0644 );
}

void closeMemory( const int shm )
{
    close( shm );
}

int createMemory( const char* name )
{
    memory_t* mem;
    int shm = shm_open( name, O_CREAT | O_RDWR, 0644 );
    if ( ftruncate( shm, SHM_SIZE ) )
    {
        printf( "error on ftruncate\n" );
        exit( EXIT_FAILURE );
    }
    mem = loadMemory( shm );
    mem->shmFd = shm;
    initMemory( mem );
    if ( munmap( mem, SHM_SIZE ) == -1 )
    {
        printf( "Error [unmap()]: " );
        exit( EXIT_FAILURE );
    }
    return shm;
}

void cleanup_memory( memory_t* mem )
{
    int fd = mem->shmFd;
    if ( munmap( mem, SHM_SIZE ) == -1 )
    {
        printf( "Error [unmap()]: " );
        exit( EXIT_FAILURE );
    }
    if ( close( fd ) == -1 )
    {
        printf( "Error prod / close() failed. " );
        exit( EXIT_FAILURE );
    }
    shm_unlink( SHM_NAME );
}