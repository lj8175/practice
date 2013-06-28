#!/bin/bash

if [ -z "$1" ]
then
  ./showBacktrace 2>&1 | tee output.scn
else
  printf "Running RTEMS Program"
  source ~/rtems-4.10-work/setenv
  pc386 -i o-optimize/stackGrowDirect.exe | tee output.scn
fi
