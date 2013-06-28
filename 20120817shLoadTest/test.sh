#!/bin/bash

if [ $# != 1 ]
then
  echo "usage: $0 unit"
  echo "1 uint == 100000"
  exit
fi

declare -i times
for((i=0;i<7;i++))
do
  times=`expr $1/$(($i+1))*100000`
  ./load.sh $(($i+1)) $times 
done
