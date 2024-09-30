#!/bin/bash

for NODES in 2 4 8
do 
  sbatch --nodes=$NODES run_monty_pi.sh
done  
