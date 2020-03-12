#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "matrix.h"

Matrix *multiply(const Matrix *matrixA, const Matrix *matrixB)
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

void *threadAction(void *args)
{
    for (int i = 0; i < 10; i++)
    {
        struct timespec waitTime;
        waitTime.tv_sec = (double)5 * ((double)rand() / RAND_MAX);

        printf("I'm THREAD and I say %d\n", i);
        printf("Now I'll sleep for %ld seconds\n", waitTime.tv_sec);

        nanosleep(&waitTime, &waitTime);
    }

    return NULL;
}

int main()
{
    srand(time(NULL));
    pthread_t thread;

    if (pthread_create(&thread, NULL, threadAction, NULL))
    {
        perror("Nie udało się utworzyć wątku\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; i++)
    {
        struct timespec waitTime;
        waitTime.tv_sec = (double)5 * ((double)rand() / RAND_MAX);

        printf("I'm MAIN and I say %d\n", i);
        printf("Now I'll sleep for %ld seconds\n", waitTime.tv_sec);

        nanosleep(&waitTime, &waitTime);
    }

    pthread_join(thread, NULL);

    // Matrix *matrixA = loadMatrixFromFile("A.txt");
    // printMatrix(matrixA);
    // puts("");

    // Matrix *matrixB = loadMatrixFromFile("B.txt");
    // printMatrix(matrixB);
    // puts("");

    // Matrix *matrixC = multiply(matrixA, matrixB);
    // printMatrix(matrixC);

    // disposeMatrix(matrixA);
    // disposeMatrix(matrixB);
    // disposeMatrix(matrixC);
    return EXIT_SUCCESS;
}
