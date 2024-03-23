#include <stdio.h>
#include <omp.h>

#define N 1000 
#define TILE_SIZE 32 


void matrixVectorMul(int matrix[N][N], int vector[N], int result[N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i += TILE_SIZE) {
        for (int j = 0; j < N; j += TILE_SIZE) {
            for (int ii = i; ii < i + TILE_SIZE; ii++) {
                for (int jj = j; jj < j + TILE_SIZE; jj++) {
                    result[ii] += matrix[ii][jj] * vector[jj];
                }
            }
        }
    }
}

int main() {
    int matrix[N][N];
    int vector[N];
    int result[N] = {0}; 

    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = i + j; 
        }
        vector[i] = i; 
    }

   
    matrixVectorMul(matrix, vector, result);

  
    printf("Resulting vector:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    return 0;
}