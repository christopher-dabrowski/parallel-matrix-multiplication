#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "matrix.h"
#include "pair.h"
#include "utils.h"

typedef struct ThreadArgument
{
    Matrix *resultMatrix;
    Pair indexRange;
} ThreadArgument;

void *threadAction(void *args)
{
    ThreadArgument *arguments = args;

    printf("Będę pracował na indeksach [%d, %d)\n", arguments->indexRange.a, arguments->indexRange.b);

    return NULL;
}

Matrix *multiply(const Matrix *matrixA, const Matrix *matrixB, const int threadCount)
{
    if (matrixA->columnCount != matrixB->rowCount)
    {
        perror("Wymiary macierzy nie pozwalaja na mnożenie\n");
        return NULL;
    }

    Matrix *result = createZerosMatrix(matrixA->rowCount, matrixB->columnCount);

    // We have to calculate values for new array from index 0 to index maxIndex
    // We can view matrix as flat array of indexes which can be splited into ranges and calculated separtatly
    const int maxIndex = result->columnCount * result->rowCount;

    Pair *ranges = splitIntoRanges(maxIndex, threadCount);
    pthread_t threads[threadCount];
    ThreadArgument threadArguments[threadCount];

    for (int i = 0; i < threadCount; i++)
    {
        threadArguments[i].resultMatrix = result;
        threadArguments[i].indexRange = ranges[i];

        if (pthread_create(&threads[i], NULL, threadAction, &threadArguments[i]))
        {
            perror("Nie udało się utworzyć wątku\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threadCount; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            perror("Nie udało się dołączyć wątku\n");
            exit(EXIT_FAILURE);
        }
    }

    // for (int index = 0; index < maxIndex; index++)
    // {
    //     Pair rowAndColumn = mapIndexToRowAndColumn(result, index);
    //     const int row = rowAndColumn.a;
    //     const int column = rowAndColumn.b;

    //     double sum = 0.;
    //     for (int i = 0; i < matrixA->columnCount; i++)
    //     {
    //         double a = getElement(matrixA, row, i);
    //         double b = getElement(matrixB, i, column);
    //         sum += a * b;
    //     }

    //     setElement(result, row, column, sum);
    // }

    free(ranges);
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

    Matrix *matrixC = multiply(matrixA, matrixB, 3);
    printMatrix(matrixC);

    disposeMatrix(matrixA);
    disposeMatrix(matrixB);
    disposeMatrix(matrixC);
    return EXIT_SUCCESS;
}
