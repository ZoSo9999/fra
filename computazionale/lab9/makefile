COMPILE = gcc -O3 -Wall

plot : RW.c
	${COMPILE} -DPLOT  RW.c -o RW.exe -lm

msd : RW.c
	${COMPILE} -DMSD  RW.c -o RW.exe -lm

histo : RW.c
	${COMPILE} -DHISTO RW.c -o RW.exe -lm
