#!/bin/bash

n=1
r0=1
P0=1
D0=0
dr=0.01
rmax=100

rm -f err_analysis.dat

STEPS="0.01, 0.001, 0.0001, 0.00001"

for STEP in ${STEPS} ; do
    ./err_analysis.exe ${n} ${r0} ${P0} ${D0} ${STEP} ${rmax} >> err_analysis.dat
done
