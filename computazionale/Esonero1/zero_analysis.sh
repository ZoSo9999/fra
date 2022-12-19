#!/bin/bash

r0=0.01
P0=1
D0=0
dr=0.0001
rmax=100

rm -f zero_analysis.dat

for ((i=0; i<=400; i++)) ; do
    ./zero_analysis.exe ${i} ${r0} ${P0} ${D0} ${dr} ${rmax} >> zero_analysis.dat
done
