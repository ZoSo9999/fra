COMPILE = gcc -O3 -Wall

ALL : EULERO CROMER CENTRAL

clean :
	rm -f oscEulero.exe oscEuleroCromer.exe oscCentral.exe Eulero.dat Cromer.dat Central.dat 

EULERO : oscillator.c
	${COMPILE} -DEULERO oscillator.c -o oscEulero.exe

CROMER : oscillator.c
	${COMPILE} -DEULEROCROMER oscillator.c -o oscEuleroCromer.exe

CENTRAL : oscillator.c
	${COMPILE} -DCENTRAL oscillator.c -o oscCentral.exe
