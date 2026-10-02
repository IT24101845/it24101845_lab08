// message2_bsend.cc - rank 0 sends three numbers using buffered sends
// mpicxx message2_bsend.cc -o message2_bsend
// mpirun -np 2 ./message2_bsend
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
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
