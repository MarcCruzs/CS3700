#!/bin/bash
#SBATCH --job-name=PI_MPI # Job name
#SBATCH --output=RUN_MONTY_PI_%j.log # Log file name
#SBATCH --partition=compute # Use computing cluster
#SBATCH --mem=1gb # Job memory request
#SBATCH --time=00:02:00 # Time limit HH:MM:SS

. /etc/profile.d/modules.sh

module load openmpi/2.1.2

/opt/openmpi-2.1.2/bin/mpirun ./monty_pi
