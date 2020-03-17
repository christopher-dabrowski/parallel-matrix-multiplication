#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "matrix.h"
#include "pair.h"
#include "utils.h"

typedef struct MultiplicationThreadArgument
{
    const Matrix *matrixA;
    const Matrix *MatrixB;
    Matrix *resultMatrix;
    Pair indexRange;
    double *sumOut;
    pthread_mutex_t *sumMutex;
} MultiplicationThreadArgument;

void *multiplicationThreadAction(void *args)
{
    MultiplicationThreadArgument *arguments = args;
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

Matrix *multiplyAndSum(const Matrix *matrixA, const Matrix *matrixB, const int threadCount, double *elementSumOut)
{
    if (matrixA->columnCount != matrixB->rowCount)
    {
        fputs("Wymiary macierzy nie pozwalaja na mnożenie\n", stderr);
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
    MultiplicationThreadArgument threadArguments[threadCount];

    // Launch treads
    for (int i = 0; i < threadCount; i++)
    {
        threadArguments[i].indexRange = ranges[i];
        threadArguments[i].matrixA = matrixA;
        threadArguments[i].MatrixB = matrixB;
        threadArguments[i].resultMatrix = result;
        threadArguments[i].sumOut = elementSumOut;
        threadArguments[i].sumMutex = &sumMutex;

        if (pthread_create(&threads[i], NULL, multiplicationThreadAction, &threadArguments[i]))
        {
            fputs("Nie udało się utworzyć wątku\n", stderr);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < threadCount; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            fputs("Nie udało się dołączyć wątku\n", stderr);
            exit(EXIT_FAILURE);
        }
    }

    free(ranges);
    return result;
}

typedef struct squareAndSumThreadArguments
{
    const Matrix *matrix;
    int thradId;
    Pair indexRange;
    double *resultsArray;
} squareAndSumThreadArguments;

void *squareAndSumThreadAction(void *args)
{
    squareAndSumThreadArguments *arguments = args;
    Matrix *matrix = arguments->matrix;
    int threadId = arguments->thradId;
    Pair indexRange = arguments->indexRange;
    double *resultsArray = arguments->resultsArray;

    double sum = 0.;
    for (int index = indexRange.a; index < indexRange.b; index++)
    {
        Pair rowAndColumn = mapIndexToRowAndColumn(matrix, index);
        const int row = rowAndColumn.a;
        const int column = rowAndColumn.b;

        double value = getElement(matrix, row, column);
        sum += value * value;
    }

    resultsArray[threadId] = sum;
    return NULL;
}

double frobieniusNorm(const Matrix *matrix, const int threadCount)
{
    double partialSums[threadCount];

    const int maxIndex = matrix->columnCount * matrix->rowCount;
    Pair *ranges = splitIntoRanges(maxIndex, threadCount);

    pthread_t threads[threadCount];
    squareAndSumThreadArguments threadArguments[threadCount];
    // Launch treads
    for (int i = 0; i < threadCount; i++)
    {
        threadArguments[i].matrix = matrix;
        threadArguments[i].thradId = i;
        threadArguments[i].indexRange = ranges[i];
        threadArguments[i].resultsArray = partialSums;

        if (pthread_create(&threads[i], NULL, squareAndSumThreadAction, &threadArguments[i]))
        {
            fputs("Nie udało się utworzyć wątku\n", stderr);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < threadCount; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            fputs("Nie udało się dołączyć wątku\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
    free(ranges);

    // Join partial sums and calculate final value
    double sum = 0.;
    for (int i = 0; i < threadCount; i++)
    {
        sum += partialSums[i];
    }

    return sqrt(sum);
}

int getThreadCount(int argc, char **argv)
{
    if (argc <= 1)
    {
        const int systemProcessors = sysconf(_SC_NPROCESSORS_ONLN);
        printf("Nie podano ilości wątków. Program użyje domyślnej wartości: %d\n\n", systemProcessors);

        return systemProcessors;
    }

    const int threadCount = atoi(argv[1]);
    if (threadCount <= 0)
    {
        fputs("Nieprawidłowa liczba wątków.\nLiczba wątków musi być dodatnią liczbą całkowitą\n", stderr);
        exit(EXIT_FAILURE);
    }

    return threadCount;
}

int main(int argc, char **argv)
{
    const int threadCount = getThreadCount(argc, argv);

    Matrix *matrixA = loadMatrixFromFile("A.txt");
    printMatrix(matrixA);
    puts("");

    Matrix *matrixB = loadMatrixFromFile("B.txt");
    printMatrix(matrixB);
    puts("");

    double elementsSum;
    Matrix *matrixC = multiplyAndSum(matrixA, matrixB, threadCount, &elementsSum);

    printMatrix(matrixC);
    printf("\nSuma elementów macierzy wynikowej: %lf\n", elementsSum);

    puts("\nNorma Frobieniusa macierzy wynikowej");
    printf("%lf\n", frobieniusNorm(matrixC, threadCount));

    disposeMatrix(matrixA);
    disposeMatrix(matrixB);
    disposeMatrix(matrixC);
    return EXIT_SUCCESS;
}
