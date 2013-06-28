#!/bin/bash

if [ $# != 2 ] 
then
echo "usage : $0 process times"
exit
fi

echo "$1 process, $2 times each"

nice -n -15 ./proc_monitor.sh > proc.txt & disown
sleep 1
b=`date '+%s'`
for ((i=0;i<$1;i++));
do
  rm -f load$i.txt
  ./L5GetRoute1 12 123 $2 >>load$i.txt &
done
wait
e=`date '+%s'`
echo use $(($e-$b)) seconds
pkill proc_monitor

declare -i count
count=$2-100000
./addnum.sh $1 $count $2 

sleep 1
sync

lines=`wc -l proc.txt | awk '{print $1}'`
awk 'BEGIN{sum=0;}
{
do{
    sum+=$11;
} while(getline>0)
print "cpu average load: "sum"/"cnt"="sum/cnt;
}
' cnt=$lines proc.txt


