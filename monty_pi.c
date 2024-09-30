#include <stdio.h>
#include <math.h>    
#include <stdlib.h>   
#include <time.h>     
#include <mpi.h>     

#define M_PI 3.14159265358979323846

double monte_carlo(int POINTS) {
    int count_inside = 0;
    double x, y, distance;

    for (int i = 0; i < POINTS; i++) {
        x = ((double)rand()) / ((double)RAND_MAX);
        y = ((double)rand()) / ((double)RAND_MAX);
        distance = pow(x, 2) + pow(y, 2);
        if (distance <= 1) {
            count_inside++;
       }
    }
    return count_inside;
}

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    char procname[MPI_MAX_PROCESSOR_NAME];
    int namelen;
    MPI_Get_processor_name(procname, &namelen);

    printf("Process %d on %s\n", myid, procname);

    int TRIALS = 5;
    int POINTS = 100;

    int local_count, global_count;
    double start_time, end_time, elapsed_time;
    double pi_estimate, delta;
    for (int i = 1; i <= 5; i++){
        start_time = MPI_Wtime();
        // initialize random seed
        srand((unsigned)(myid) + time(0));

        // monte carlo process
        local_count = monte_carlo(POINTS);
        global_count = 0;

        MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (myid == 0) {
            pi_estimate = (4.0 * global_count) / (POINTS * numprocs);
            delta = fabs(M_PI - pi_estimate);
            end_time = MPI_Wtime();
            elapsed_time = end_time - start_time;

            printf("%d NODES - %d POINTS: ESTIMATED PI = %f, DELTA = %.4f, TIME = %.6f seconds\n", numprocs, POINTS, pi_estimate, delta, elapsed_time);
        }

        POINTS *= 10;
    }

    MPI_Finalize();
    return 0;
}
