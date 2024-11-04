#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <climits>

#define ARRAY_SIZE 1000000

int main (int argc,  char *argv[]) {

   int myid, numprocs;
   int namelen;
   int* numbers = new int[ARRAY_SIZE];
   char processor_name[MPI_MAX_PROCESSOR_NAME];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);

   printf("Process %d on %s\n", myid, processor_name);

   srand((unsigned)(myid) + time(0));

   for (int i = 0; i < ARRAY_SIZE; i++) {
      numbers[i] = rand();
   }

   int s = (int)floor(ARRAY_SIZE / numprocs);
   int s0 = s + ARRAY_SIZE % numprocs;

   int startIndex = (myid == 0) ? 0 : s0 + (myid - 1) * s;
   int endIndex = (myid == 0) ? s0 : startIndex + s;

   double startwtime;
   if (myid == 0) {
      startwtime = MPI_Wtime();
   }

   int i;
   int part_sum = 0;
   int local_min = INT_MAX;
   int local_max = INT_MIN;

   // Compute part_sum, local_min, and local_max for each process
   for (i = startIndex; i < endIndex; i++) {
      part_sum += numbers[i];

      if (numbers[i] < local_min) {
         local_min = numbers[i];
      }
      if (numbers[i] > local_max) {
         local_max = numbers[i];
      }
   }

   printf("Process %d - startIndex %d endIndex %d; part_sum %d; local_min %d; local_max %d\n",
          myid, startIndex, endIndex - 1, part_sum, local_min, local_max);

   // Variables to hold global values
   int min = INT_MAX;
   int max = INT_MIN;
   int sum = 0;

   // Reduce the values across all processes
   MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Reduce(&local_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
   MPI_Reduce(&local_max, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

   if (myid == 0) {
      double runTime = MPI_Wtime() - startwtime;
      printf("Execution time (sec) = %f\nsum = %d\nmin = %d\nmax = %d\n",
             runTime, sum, min, max);
   }

   delete[] numbers;

   MPI_Finalize();
}
