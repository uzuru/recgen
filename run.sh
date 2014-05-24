#!/bin/bash
gcc -std=c99 -Wall recgen.c -o recgen -lm 
rm test*
for x in {1..1000} ; do ./recgen test$x.ppm $x; 

if [ -f test$x.ppm ]; then
	pnmtopng test$x.ppm > test$x.png; 
fi

rm test$x.ppm;
done
