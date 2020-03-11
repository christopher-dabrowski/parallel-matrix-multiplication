#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "matrix.h"

int main()
{
    Matrix matrix = loadMatrixFromFile("A.txt");
    printMatrix(matrix);

    disposeMatrix(matrix);
    return EXIT_SUCCESS;
}
