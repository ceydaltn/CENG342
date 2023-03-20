
#include <hellomake.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: ./matrix-vector <n> <m> <output_file>\n");
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    char *output_file = argv[3];

    double **matrix = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *) calloc(m, sizeof(double));
    }

    double *vector = (double *) malloc(m * sizeof(double));
    double *result = (double *) malloc(n * sizeof(double));


    // Set random seed based on student ID
    
    srand((unsigned short)19050111061);


    // Fill matrix with random double values between [1.0 - 100.0]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = ((double) rand() / RAND_MAX) * 99.0 + 1.0;
        }
    }

    // Create random vector
    for (int i = 0; i < m; i++) {
        vector[i] = ((double) rand() / RAND_MAX) * 99.0 + 1.0;
    }

    // Perform matrix-vector multiplication
    matrix_vector_multiplication(matrix, vector, n, m, result);

    // Write solution to file
    FILE *fp;
    fp = fopen(output_file, "w");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%.2f\n", result[i]);
    }
    fclose(fp);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
