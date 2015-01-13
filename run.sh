#!/bin/bash
rm recgen
gcc -std=c99 -Wall recgen.c -o recgen -lm 
rm output/test*
for x in {1000..5000} ; do 

procount=$(ps -A |grep -c recgen)
while [ $procount -ge 4 ]; do  #wait until cores become free
echo "waiting... $procount $x"
procount=$(ps -A |grep -c recgen)
sleep 1
done 

{
./recgen test$x.ppm $x 

if [ -f test$x.ppm ]; then
	pnmtopng test$x.ppm > output/test$x.png; 
fi

rm test$x.ppm;
} &

done
