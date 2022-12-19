#!/bin/bash

M=1
K=10
TMAX=10
V0=1
X0=0
STEPS="0.01 0.005 0.001 0.0005 0.0002 0.0001"

rm -f Eulero_err.dat
for STEP in ${STEPS} ; do
    ./oscEulero.exe ${TMAX} ${STEP} ${X0} ${V0} ${M} ${K} | grep ERR | awk '{printf "%16s %16s\n", $3, $4;}' >> Eulero_err.dat
done


rm -f Cromer_err.dat
for STEP in ${STEPS} ; do
    ./oscEuleroCromer.exe ${TMAX} ${STEP} ${X0} ${V0} ${M} ${K} | grep ERR | awk '{printf "%16s %16s\n", $3, $4;}' >> Cromer_err.dat
done


rm -f Central_err.dat
for STEP in ${STEPS} ; do
    ./oscCentral.exe ${TMAX} ${STEP} ${X0} ${V0} ${M} ${K} | grep ERR | awk '{printf "%16s %16s\n", $3, $4;}' >> Central_err.dat
done
