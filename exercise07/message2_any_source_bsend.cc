// Exercise 7: Exercise 6 rewritten using MPI_Bsend and MPI_ANY_SOURCE.
// mpicxx -std=c++11 -Wall -Wextra message2_any_source_bsend.cc -o message2_any_source_bsend
// mpirun -np 2 ./message2_any_source_bsend
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number;
    const int message_count = 3;
    int packed_size;
    MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &packed_size);
    const int buffer_size =
        message_count * (packed_size + MPI_BSEND_OVERHEAD);
    std::vector<char> buffer(buffer_size);

    if (rank == 0) {
        MPI_Buffer_attach(buffer.data(), buffer_size);
        for (int i = 0; i < message_count; i++) {
            number = i * 10;
            MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << number << "\n";
        }

        void* attached_buffer;
        int attached_size;
        MPI_Buffer_detach(&attached_buffer, &attached_size);
    } else if (rank == 1) {
        for (int i = 0; i < message_count; i++) {
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
            std::cout << "Process 1 received " << number
                      << " from process " << status.MPI_SOURCE << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
