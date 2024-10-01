#!/bin/bash
#SBATCH --job-name=COMPILE_LAB02_MPI     
#SBATCH --output=COMPILE_MINMAX_CPP_%j.log 
#SBATCH --partition=compute          
#SBATCH --mem=1gb                    
#SBATCH --time=00:05:00              

. /etc/profile.d/modules.sh

module load openmpi/2.1.2

mpiCC ./minmax.cpp -o minmax
