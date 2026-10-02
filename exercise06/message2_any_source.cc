// message2_any_source.cc - rank 0 sends three numbers; rank 1 accepts any source
// mpicxx -std=c++11 -Wall -Wextra message2_any_source.cc -o message2_any_source
// mpirun -np 2 ./message2_any_source
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number;
    for (int i = 0; i < 3; i++) {
        if (rank == 0) {
            number = i * 10;
            MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << number << "\n";
        } else if (rank == 1) {
            MPI_Status status;
            MPI_Recv(
                &number,
                1,
                MPI_INT,
                MPI_ANY_SOURCE,
                0,
                MPI_COMM_WORLD,
                &status
            );
            std::cout << "Process " << rank << " received " << number
                      << " from process " << status.MPI_SOURCE << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
