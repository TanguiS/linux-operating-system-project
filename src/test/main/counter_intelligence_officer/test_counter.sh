#!/bin/bash
{
    sleep 2
    echo " "
    echo " "
    echo " "
    sleep 1

    {
        for (( i=1; i<= 500; i++ )); do
            pkill -SIGCONT -f counter_intelligence_officer.out
            sleep 0.2
        done
    }
}