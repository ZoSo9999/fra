COMPILE = gcc -O3 -Wall

genera : generator.c
	${COMPILE} generator.c -o generator.exe -lm
	./generator.exe 100 10 1 > histogram.dat

