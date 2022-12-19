#!/bin/bash

start=0
dx=31.4
pi=`echo "h=10;4*a(1)" | bc -l`
echo $pi

echo "var=219.22;var*=0.001;var" | bc 




x=${start}
v=${start}
a=0.5
b=0

#echo ${dx}

#rm -f attrazione.dat

#./attrazione.exe ${a} ${b} >> attrazione.dat


#for ((i=0; i<=20; i++)) ; do
 #  x+=${i}*${dx}
  #  echo ${x}
   # for ((j=0; j<=20; j++)) ; do
#	v+=${j}*${dx}
#	./attrazione.exe ${x} ${v} >> attrazione.dat
#   done
#done
