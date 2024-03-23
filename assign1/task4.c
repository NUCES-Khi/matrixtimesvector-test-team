#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000 


void matrixVectorMul(int matrix[N][N], int vector[N], int result[N], int rows_per_proc) {
    for (int i = 0; i < rows_per_proc; i++) {
        result[i] = 0;
        for (int j = 0; j < N; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int matrix[N][N];
    int vector[N];
    int result[N];
    int rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows_per_proc = N / size;

    
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = i + j; 
            }
            vector[i] = i; 
        }
    }

    
    MPI_Scatter(matrix, rows_per_proc * N, MPI_INT, matrix, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, N, MPI_INT, 0, MPI_COMM_WORLD);

   
    matrixVectorMul(matrix, vector, result, rows_per_proc);

   
    MPI_Gather(result, rows_per_proc, MPI_INT, result, rows_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

   
    if (rank == 0) {
        printf("Resulting vector:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}