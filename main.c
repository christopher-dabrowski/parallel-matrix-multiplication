#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "matrix.h"

Matrix *multiply(const Matrix *matrixA, const Matrix *matrixB)
{
    Matrix *result = createZerosMatrix(matrixA->columnCount, matrixB->rowCount);

    return result;
}

int main()
{
    Matrix *matrixA = loadMatrixFromFile("A.txt");
    printMatrix(matrixA);
    puts("");

    Matrix *matrixB = loadMatrixFromFile("B.txt");
    printMatrix(matrixB);

    disposeMatrix(matrixA);
    disposeMatrix(matrixB);
    return EXIT_SUCCESS;
}
