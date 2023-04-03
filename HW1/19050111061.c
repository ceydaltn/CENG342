//Ceyda Altın 
//19050111061
//In this program we parallelize our matrix-vector multiplication program by using MPI

#include <hellomake.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LARGE_MATRIX_SIZE (id % 10000)
#define SMALL_MATRIX_SIZE (100 + (id % 100))


int main(int argc, char *argv[]) {
    int id, size;
    double *large_matrix = NULL, *small_matrix = NULL, *vector = NULL, *result = NULL;
    int i, j;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (id == 0) {
        // Allocate memory for large and small matrices, and vector
        large_matrix = (double *) malloc(LARGE_MATRIX_SIZE * LARGE_MATRIX_SIZE * sizeof(double));
        small_matrix = (double *) malloc(SMALL_MATRIX_SIZE * SMALL_MATRIX_SIZE * sizeof(double));
        vector = (double *) malloc(LARGE_MATRIX_SIZE * sizeof(double));
        result = (double *) malloc(LARGE_MATRIX_SIZE * sizeof(double));

        // Initialize large and small matrices, and vector
        for (i = 0; i < LARGE_MATRIX_SIZE; i++) {
            for (j = 0; j < LARGE_MATRIX_SIZE; j++) {
                large_matrix[i * LARGE_MATRIX_SIZE + j] = rand() / (double) RAND_MAX;
            }
        }

        for (i = 0; i < SMALL_MATRIX_SIZE; i++) {
            for (j = 0; j < SMALL_MATRIX_SIZE; j++) {
                small_matrix[i * SMALL_MATRIX_SIZE + j] = rand() / (double) RAND_MAX;
            }
        }

        for (i = 0; i < LARGE_MATRIX_SIZE; i++) {
            vector[i] = rand() / (double) RAND_MAX;
        }
    }

    start_time = MPI_Wtime();

    // Broadcast vector to all processes
    MPI_Bcast(vector, LARGE_MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Allocate memory for submatrices
    int block_size = LARGE_MATRIX_SIZE / size;
    double *sub_large_matrix = (double *) malloc(block_size * LARGE_MATRIX_SIZE * sizeof(double));
    double *sub_result = (double *) malloc(block_size * sizeof(double));

    // Scatter large matrix to all processes
    MPI_Scatter(large_matrix, block_size * LARGE_MATRIX_SIZE, MPI_DOUBLE,
                sub_large_matrix, block_size * LARGE_MATRIX_SIZE, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Compute sub-result
    for (i = 0; i < block_size; i++) {
        sub_result[i] = 0.0;
        for (j = 0; j < LARGE_MATRIX_SIZE; j++) {
            sub_result[i] += sub_large_matrix[i * LARGE_MATRIX_SIZE + j] * vector[j];
        }
    }

    // Gather sub-results to rank 0
    MPI_Gather(sub_result, block_size, MPI_DOUBLE,
               result, block_size, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (id == 0) {
        // Compute final result
        matrix_vector_multiply(small_matrix, result, vector, SMALL_MATRIX_SIZE);

        // Print elapsed time
        printf("Elapsed time: %f seconds for parallel mxv with %d processes\n", end_time - start_time, size);

        // Free memory
        free(large_matrix);
        free(small_matrix);
        free(vector);
        free(result);
        free(sub_large_matrix);
        free(sub_result);
    }

    MPI_Finalize();
    return 0;
}
