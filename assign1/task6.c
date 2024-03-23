#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000 // Size of the matrix and vector
#define TILE_SIZE 100 // Tile size for tiling the matrix

void matrixVectorMulTile(int matrix[TILE_SIZE][N], int vector[N], int result[TILE_SIZE]) {
    for (int i = 0; i < TILE_SIZE; i++) {
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
    int result[TILE_SIZE];
    int rows_per_proc;
    int tile_count;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows_per_proc = N / size;
    tile_count = rows_per_proc / TILE_SIZE;

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

    for (int t = 0; t < tile_count; t++) {
        int start_row = t * TILE_SIZE;
        matrixVectorMulTile(matrix + start_row, vector, result);
        MPI_Gather(result, TILE_SIZE, MPI_INT, matrix + start_row, TILE_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("Resulting vector:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", matrix[0][i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}