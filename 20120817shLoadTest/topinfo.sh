#!/bin/bash 

# process name which should be monitored
modules=("init")

for module in ${modules[@]}
do
    t=`date '+%F %X'`
    sts=`top -bn 1 | grep $module`
    echo  $t $sts
done
