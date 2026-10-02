#include <mpi.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FIRST_NUMBER UINT64_C(1)
#define LAST_NUMBER UINT64_C(10000000)

int main(int argc, char **argv)
{
    int rank;
    int process_count;
    uint64_t total = 0;
    uint64_t local_sum = 0;
    uint64_t start;
    uint64_t end;
    uint64_t numbers;
    uint64_t base_count;
    uint64_t remainder;
    uint64_t expected;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    numbers = LAST_NUMBER - FIRST_NUMBER + 1;
    base_count = numbers / (uint64_t)process_count;
    remainder = numbers % (uint64_t)process_count;

    /*
     * Give the first `remainder` processes one additional number so that
     * every number in the requested range is included exactly once.
     */
    start = FIRST_NUMBER + (uint64_t)rank * base_count;
    if ((uint64_t)rank < remainder) {
        start += (uint64_t)rank;
        end = start + base_count;
    } else {
        start += remainder;
        end = start + base_count - 1;
    }

    for (uint64_t number = start; number <= end; ++number) {
        local_sum += number;
    }

    MPI_Reduce(
        &local_sum,
        &total,
        1,
        MPI_UINT64_T,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {
        expected = numbers * (FIRST_NUMBER + LAST_NUMBER) / 2;
        printf("Sum of numbers from %" PRIu64 " to %" PRIu64 ": %" PRIu64 "\n",
               FIRST_NUMBER, LAST_NUMBER, total);
        printf("Expected result: %" PRIu64 "\n", expected);

        if (total != expected) {
            fprintf(stderr, "Error: calculated sum does not match expected result.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
