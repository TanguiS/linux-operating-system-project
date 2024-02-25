#!/bin/bash
make
./bin/spy_simulation.out 0.1 &
{
    sleep 2
    echo " "
    echo " "
    echo " "
    sleep 1
    ./bin/citizen_manager.out &
    {
        for (( i=1; i<= 50; i++ )); do
            pkill -SIGCONT -f citizen_manager.out
            sleep 0.1
        done
        pkill -SIGTERM -f citizen_manager.out
    }
    pkill -f spy_simulation.out
}
