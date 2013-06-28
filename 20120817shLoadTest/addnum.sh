#!/bin/bash

if [ $# != 3 ]
then
echo "usage : $0 Count Begin End"
exit
fi

declare -i count
count=$1-1
#echo "addnum load0-$count.txt, from $2 to $3"

for ((i=0;i<$1;i++))
do
awk -F "[:=]" 'BEGIN {sum=0; all=0; flag=false}
{
do{
  if($2==" "e) flag=!flag;
    if($2!=""&&flag){
    #print $1" "$2;
      all+=$1*$2;
      sum+=$2;
    }
  if($2==" "b)
  {
    #print NR;
    flag=!flag;
  }
} while (getline > 0);
print sum; 
print all;
}' b=$2 e=$3 load$i.txt
done | awk 'BEGIN{flag=false;sum=0;all=0;}
{
do{
  if(!flag)
  { 
    sum+=$1;
  }
  else
  {
    all+=$1;
  }
  flag = !flag;
} while(getline>0)
print "not pre times: "sum;
print "all     times: "all;
}
'
for ((i=0;i<$1;i++))
do
 echo "process $i :" `tail -1 load$i.txt`
done
