// Demonstrates a source/destination mismatch.
// Rank 0 sends to rank 1, but rank 1 waits for a message from rank 1.
// mpicxx message_mismatch.cc -o message_mismatch
// mpirun -np 2 ./message_mismatch
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = 42;
    if (rank == 0) {
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent " << number << "\n";
    } else if (rank == 1) {
        // This does not match rank 0's send: the expected source is wrong.
        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 1 received " << number << "\n";
    }

    MPI_Finalize();
    return 0;
}
