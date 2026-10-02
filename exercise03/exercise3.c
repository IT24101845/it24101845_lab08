#include <inttypes.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_TRIALS UINT64_C(10000000)
#define RANDOM_SEED UINT32_C(12345)

static uint32_t next_random(uint32_t *state)
{
    *state = *state * UINT32_C(1664525) + UINT32_C(1013904223);
    return *state;
}

int main(int argc, char **argv)
{
    int rank;
    int process_count;
    uint64_t base_trials;
    uint64_t remainder;
    uint64_t local_trials;
    uint64_t local_hits = 0;
    uint64_t total_hits = 0;
    uint32_t seed;
    double pi;
    double error;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    base_trials = TOTAL_TRIALS / (uint64_t)process_count;
    remainder = TOTAL_TRIALS % (uint64_t)process_count;
    local_trials = base_trials + ((uint64_t)rank < remainder ? 1 : 0);
    seed = RANDOM_SEED + (uint32_t)rank;

    for (uint64_t trial = 0; trial < local_trials; ++trial) {
        double x = 2.0 * (double)next_random(&seed) / 4294967296.0 - 1.0;
        double y = 2.0 * (double)next_random(&seed) / 4294967296.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            ++local_hits;
        }
    }

    MPI_Reduce(
        &local_hits,
        &total_hits,
        1,
        MPI_UINT64_T,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {
        pi = 4.0 * (double)total_hits / (double)TOTAL_TRIALS;
        error = pi - 3.14159265358979323846;
        if (error < 0.0) {
            error = -error;
        }

        printf("Monte Carlo trials: %" PRIu64 "\n", TOTAL_TRIALS);
        printf("Points inside circle: %" PRIu64 "\n", total_hits);
        printf("Calculated pi: %.15f\n", pi);
        printf("Absolute error: %.15f\n", error);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
