#! /bin/bash
make
read -p "which platform ? linux:1 rtems:2  :" var
if [ $var = "1" ]
then
#  ./linux | tee linux.scn
  ./linux
else
#  ~/rtems-4.10-work/rtems-testing/bin/pc386 -i rtems | tee rtems.scn
  ~/rtems-4.10-work/rtems-testing/bin/pc386 -i rtems
fi
