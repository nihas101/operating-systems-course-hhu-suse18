#include <stdlib.h>

#define getAddress(matrix_pointer, current_row, current_column, columns) (matrix_pointer + current_row * columns + current_column)


void initializeMatrix(long long* matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			*getAddress(matrix, i, j, cols) = rand() % 100;
		}
	}
}

void multiplyMatrices(long long* matrixA, long long* matrixB, long long* matrixC, int rowsA, int colsA, int colsB) {
	for (int i = 0; i < rowsA; i++) {
		for(int j = 0; j < colsB; j++) {
			for (int k = 0; k < colsA; k++) {
				*getAddress(matrixC, i, j, colsB) += *getAddress(matrixA, i, k, colsA) * *getAddress(matrixB, k, j, colsB);
			}
		}
    }
}