COMPILE = gcc -O3 -Wall

uniform : generator.c
	${COMPILE} generator.c -DUNIFORM -o uniform.exe -lm
	./uniform.exe 100000 100 1 0 1 > histogram.dat

gaussian : generator.c
	${COMPILE} generator.c -DGAUSSIAN -o gaussian.exe -lm
	./gaussian.exe 100000 100 1 0 1 > histogram.dat

randu : generator.c
	${COMPILE} generator.c -DRANDU -o randu.exe -lm
	./randu.exe 10000 100 2 0 1 > randu.dat
