#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int n, sqrt_n;
    int terminator = -1;
    double start_time, end_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) {
            printf("Usage: %s <upper_bound>\n", argv[0]);
        }
        MPI_Finalize();
        exit(1);
    }

    n = atoi(argv[1]);
    sqrt_n = (int) sqrt(n);

    // Start timing the execution
    if (rank == 0) start_time = MPI_Wtime();

    int current_prime = 2 + rank;  // Assign each process a starting prime
    if (rank == 0) {
        // Process 0 initializes the pipeline
        printf("Prime found: 2\n");

        //Skip 2 and all even numbers
        for (int num = 3; num <= n; num += 2) {
            MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        // Send terminator message to the first process
        MPI_Send(&terminator, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        int received_number;
        int prime_found = 1;  // Boolean to indicate if prime has been found

        // Pipeline processing loop
        while (1) {
            MPI_Recv(&received_number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Check for the termination message
            if (received_number == terminator) {
                if (rank + 1 < size) {
                    MPI_Send(&terminator, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
                break;
            }

            // Check if the received number is a multiple of the current prime
            if (received_number % current_prime != 0) {
                if (rank + 1 < size) {
                    MPI_Send(&received_number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                } else {
                    // Last process, print the prime if no more filtering stages
                    printf("Prime found: %d\n", received_number);
                }
            }
        }
    }

    // Stop timing the execution
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
