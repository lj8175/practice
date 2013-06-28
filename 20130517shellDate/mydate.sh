#/bin/bash
for((i=1;i<=$1;i++));
do
    dt=`date +%Y%m%d -d "$(($1-$i)) day"`
    echo $dt:"十条记忆；工作"
done

echo "done"
