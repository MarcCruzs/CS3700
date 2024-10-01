#!/bin/bash
#SBATCH --job-name=COMPILE_MPI     
#SBATCH --output=COMPILE_MONTY_PI_%j.log 
#SBATCH --partition=compute          
#SBATCH --mem=1gb                    
#SBATCH --time=00:05:00              
. /etc/profile.d/modules.sh

module load openmpi/2.1.2

mpicc -std=c99 -o monty_pi monty_pi.c -lm
