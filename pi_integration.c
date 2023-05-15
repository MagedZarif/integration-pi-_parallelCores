#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) 
{

    int num_steps = 1000000, num_procs, my_rank;

    double step, x, pi, sum = 0.0, slav_sum = 0.0;

	int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    int steps_per_proc = ceil(num_steps / num_procs);

    if (my_rank == 0) 
	{
        step = 1.0/(double)num_steps;
    }

    MPI_Bcast(&step, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&steps_per_proc, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    int start = my_rank * (steps_per_proc);
    int end = (my_rank + 1) * (steps_per_proc);

    if (my_rank == num_procs - 1) 
	{
        end = num_steps;
    }


    for (i = start; i < end; i++) 
	{
        x = ((double)i + 0.5) * step;
        slav_sum += 4.0/(1.0+x*x);
    }

    MPI_Reduce(&slav_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) 
	{
        pi = step * sum;
        printf("%.20f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
