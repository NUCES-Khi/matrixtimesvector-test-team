#include <stdio.h>
#include <omp.h>

#define N 1000 


void matrixVectorMul(int matrix[N][N], int vector[N], int result[N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        result[i] = 0;
        for (int j = 0; j < N; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main() {
    int matrix[N][N];
    int vector[N];
    int result[N];

    
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