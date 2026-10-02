#include <cstdio>
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name(name, &len);

    if (rank == 0) {
        printf("Hello World! From rank %d machine %s\n", rank, name);
    } else {
        printf("Just a normal process From rank %d machine %s\n", rank, name);
    }

    MPI_Finalize();
    return 0;
}
