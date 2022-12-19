#!/bin/bash

POINTS=200
RHO=0.1
L=40
STORIES=2000
STEP=10



for((i=1; i<=${POINTS}; i++)) ; do
    let T=${i}*${STEP}
    ./reticular.exe 0.1 ${L} ${T} ${STORIES} >> dati/diffusione.dat
done

