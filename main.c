#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "matrix.h"

int main()
{
    Matrix *matrix = loadMatrixFromFile("A.txt");
    printMatrix(matrix);
    puts("");

    disposeMatrix(matrix);

    Matrix *zeros = createZerosMatrix(2, 3);
    setElement(zeros, 0, 2, 10.5);
    printMatrix(zeros);

    disposeMatrix(zeros);
    return EXIT_SUCCESS;
}
