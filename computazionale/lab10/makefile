COMPILE = gcc -O3 -Wall

reticular : reticular.c
	${COMPILE} reticular.c -o reticular.exe -lm

debug :	reticular.c
	${COMPILE} -DDEBUG -DLEGENDA reticular.c -o reticular.exe -lm
