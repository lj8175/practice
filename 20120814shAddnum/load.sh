#!/bin/bash

if [ $# != 2 ] 
then
echo "usage : $0 process times"
exit
fi

echo "$1 process, $2 times each"
b=`date '+%s'`
for ((i=0;i<$1;i++));
do
rm -f load$i.txt
./Route1 12 123 $2 >>load$i.txt &
done
wait
e=`date '+%s'`
echo use $(($e-$b)) seconds
declare -i count
count=$2-100000
./addnum.sh $1 $count $2 
