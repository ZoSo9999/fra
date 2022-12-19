COMPILE = gcc -O3 -Wall

ALL : attrazione

simulation : integrator.c
	${COMPILE} -DSIMULATION integrator.c -o simulation.exe -lm
	./simulation.exe
poincare : integrator.c
	${COMPILE} -DPOINCARE integrator.c -o poincare.exe -lm
	./poincare.exe
attrazione :  integrator.c
	${COMPILE} -DATTRAZIONE integrator.c -o attrazione.exe -lm

