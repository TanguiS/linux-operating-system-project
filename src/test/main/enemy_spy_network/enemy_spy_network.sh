#!/bin/bash
make distclean
make
./bin/spy_simulation.out 0.1 &
{
    sleep 2
    echo " "
    echo " "
    echo " "
    sleep 1
    ./bin/enemy_spy_network.out &
    {
        for (( i=1; i<= 200; i++ )); do
            pkill -SIGCONT -f enemy_spy_network.out
            sleep 0.2
        done
        pkill -SIGTERM -f enemy_spy_network.out
    }
    pkill -f spy_simulation.out
}
