COMPILE = gcc -O3 -Wall

ALL : cintegrator integrator

cintegrator : integrator.c
	${COMPILE} integrator.c -o integrator.exe -lm
integrator : integrator.exe
	./integrator.exe
