#!/bin/bash

for THREADS in 2 4 8
do 
  for POINTS in 100 1000 10000 100000 1000000
  do
     ./monte_pi.exe $THREADS $POINTS
  done   
done
