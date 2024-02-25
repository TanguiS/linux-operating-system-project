#!/bin/bash
make
./bin/spy_simulation.out 0.1 &
{
    sleep 2
    echo " "
    echo " "
    echo " "
    ./bin/case_officer_manager.out &
    sleep 1
    ./bin/enemy_country.out &
    sleep 1
    {
        for (( i=1; i<= 100; i++ )); do
            pkill -SIGCONT -f case_officer_manager.out
            sleep 0.1
        done
        sleep 1
        pkill -SIGTERM -f case_officer_manager.out
        pkill -SIGTERM -f enemy_country.out
    }
    pkill -f spy_simulation.out
}
