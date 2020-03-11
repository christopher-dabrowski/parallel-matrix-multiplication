#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "matrix.h"

Matrix *multiply(const Matrix *matrixA, const Matrix *matrixB)
{
    Matrix *result = createZerosMatrix(matrixA->rowCount, matrixB->columnCount);

    // We have to calculate values for new array from index 0 to index maxIndex
    // We can view matrix as flat array of indexes which can be splited into ranges and calculated separtatly
    const int maxIndex = result->columnCount * result->rowCount;

    for (int index = 0; index < maxIndex; index++)
    {
        Pair rowAndColumn = mapIndexToRowAndColumn(result, index);
        const int row = rowAndColumn.a;
        const int column = rowAndColumn.b;

        double sum = 0.;
        for (int i = 0; i < matrixA->columnCount; i++)
        {
            double a = getElement(matrixA, row, i);
            double b = getElement(matrixB, i, column);
            sum += a * b;
        }

        setElement(result, row, column, sum);
    }

    return result;
}

int main()
{
    Matrix *matrixA = loadMatrixFromFile("A.txt");
    printMatrix(matrixA);
    puts("");

    Matrix *matrixB = loadMatrixFromFile("B.txt");
    printMatrix(matrixB);
    puts("");

    Matrix *matrixC = multiply(matrixA, matrixB);
    printMatrix(matrixC);

    disposeMatrix(matrixA);
    disposeMatrix(matrixB);
    disposeMatrix(matrixC);
    return EXIT_SUCCESS;
}
