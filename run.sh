#!/bin/bash
rm recgen
gcc -std=c99 -Wall recgen.c -o recgen -lm 
rm output/test*
for x in {1000..5000} ; do ./recgen test$x.ppm $x; 

if [ -f test$x.ppm ]; then
	pnmtopng test$x.ppm > output/test$x.png; 
fi

rm test$x.ppm;
done
