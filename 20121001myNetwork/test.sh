#!/bin/bash

for ((i=0; i<10000; i++))
do
    ./client localhost 8888
#    sleep 0.00001
done
