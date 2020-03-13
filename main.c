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
    const Matrix *matrixA;
    const Matrix *MatrixB;
    Matrix *resultMatrix;
    Pair indexRange;
    double *sumOut;
    pthread_mutex_t *sumMutex;
} ThreadArgument;

void *threadAction(void *args)
{
    ThreadArgument *arguments = args;
    Pair indexes = arguments->indexRange;
    const Matrix *matrixA = arguments->matrixA;
    const Matrix *matrixB = arguments->MatrixB;
    Matrix *resultMatrix = arguments->resultMatrix;
    double *sumOut = arguments->sumOut;
    pthread_mutex_t *sumMutex = arguments->sumMutex;

#ifdef DEBUG
    printf("Będę pracował na indeksach [%d, %d)\n", indexes.a, indexes.b);
#endif
    for (int index = indexes.a; index < indexes.b; index++)
    {
        Pair rowAndColumn = mapIndexToRowAndColumn(resultMatrix, index);
        const int row = rowAndColumn.a;
        const int column = rowAndColumn.b;

        double sum = 0.;
        for (int i = 0; i < matrixA->columnCount; i++)
        {
            double a = getElement(matrixA, row, i);
            double b = getElement(matrixB, i, column);
            sum += a * b;
        }

        setElement(resultMatrix, row, column, sum);

        if (sumOut)
        {
            pthread_mutex_lock(sumMutex);
            *sumOut += sum;
            pthread_mutex_unlock(sumMutex);
        }
    }

    return NULL;
}

Matrix *multiply(const Matrix *matrixA, const Matrix *matrixB, const int threadCount, double *elementSumOut)
{
    if (matrixA->columnCount != matrixB->rowCount)
    {
        perror("Wymiary macierzy nie pozwalaja na mnożenie\n");
        return NULL;
    }

    Matrix *result = createZerosMatrix(matrixA->rowCount, matrixB->columnCount);
    if (elementSumOut)
        *elementSumOut = 0.;
    pthread_mutex_t sumMutex = PTHREAD_MUTEX_INITIALIZER;

    // We have to calculate values for new array from index 0 to index maxIndex
    // We can view matrix as flat array of indexes which can be splited into ranges and calculated separtatly
    const int maxIndex = result->columnCount * result->rowCount;

    Pair *ranges = splitIntoRanges(maxIndex, threadCount);
    pthread_t threads[threadCount];
    ThreadArgument threadArguments[threadCount];

    // Lunch treads
    for (int i = 0; i < threadCount; i++)
    {
        threadArguments[i].indexRange = ranges[i];
        threadArguments[i].matrixA = matrixA;
        threadArguments[i].MatrixB = matrixB;
        threadArguments[i].resultMatrix = result;
        threadArguments[i].sumOut = elementSumOut;
        threadArguments[i].sumMutex = &sumMutex;

        if (pthread_create(&threads[i], NULL, threadAction, &threadArguments[i]))
        {
            perror("Nie udało się utworzyć wątku\n");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < threadCount; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            perror("Nie udało się dołączyć wątku\n");
            exit(EXIT_FAILURE);
        }
    }

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

    double elementsSum;
    Matrix *matrixC = multiply(matrixA, matrixB, 3, &elementsSum);

    printMatrix(matrixC);
    printf("\nSuma elementów macierzy wynikowej: %lf\n", elementsSum);

    disposeMatrix(matrixA);
    disposeMatrix(matrixB);
    disposeMatrix(matrixC);
    return EXIT_SUCCESS;
}
