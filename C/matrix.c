#include "matrix.h"

int calculateIndex(const Matrix *matrix, const int rowNumber, const int columnNumber)
{
    return rowNumber * matrix->columnCount + columnNumber;
}

Pair mapIndexToRowAndColumn(const Matrix *matrix, const int index)
{
    Pair result;
    result.a = index / matrix->columnCount;
    result.b = index % matrix->columnCount;

    return result;
}

Matrix *createMatrix(const int rowCount, const int columnCount, double data[])
{
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rowCount = rowCount;
    matrix->columnCount = columnCount;

    matrix->data = malloc(rowCount * columnCount * sizeof(*matrix->data));
    memcpy(matrix->data, data, rowCount * columnCount * sizeof(*matrix->data));

    return matrix;
}

Matrix *createZerosMatrix(const int rowCount, const int columnCount)
{
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rowCount = rowCount;
    matrix->columnCount = columnCount;

    matrix->data = calloc(rowCount * columnCount, sizeof(*matrix->data));

    return matrix;
}

void disposeMatrix(Matrix *matrix)
{
    free(matrix->data);
    free(matrix);
}

Matrix *loadMatrixFromFile(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        fputs("Nie udało się otworzyć pliku\n", stderr);
        exit(EXIT_FAILURE);
    }

    int rowCount, columnCount;
    if (fscanf(file, "%d", &rowCount) != 1)
    {
        fclose(file);
        fputs("Nie udało się wczytać liczby wierszy\n", stderr);
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", &columnCount) != 1)
    {
        fclose(file);
        fputs("Nie udało się wczytać liczby kolumn\n", stderr);
        exit(EXIT_FAILURE);
    }

    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rowCount = rowCount;
    matrix->columnCount = columnCount;
    matrix->data = malloc(rowCount * columnCount * sizeof(*matrix->data));

    for (int row = 0; row < rowCount; row++)
    {
        for (int column = 0; column < columnCount; column++)
        {
            const int index = calculateIndex(matrix, row, column);
            if (fscanf(file, "%lf", &matrix->data[index]) != 1)
            {
                fclose(file);
                free(matrix->data);
                fprintf(stderr, "Nie udało się wczytać elementu macierzy [%d, %d]\n", row + 1, column + 1);
                exit(EXIT_FAILURE);
            }

            // #ifdef DEBUG
            //             printf("Wczytuję element row: %d column: %d\n", row, column);
            //             printf("Indeks wczytanego elemenut: %d\twartość: %lf\n", index, matrix->data[index]);
            // #endif
        }
    }

    fclose(file);
    return matrix;
}

// Indexes are numbered from 0 to n-1
double getElement(const Matrix *matrix, const int rowNumber, const int columnNumber)
{
    const int index = calculateIndex(matrix, rowNumber, columnNumber);
    return matrix->data[index];
}

// Indexes are numbered from 0 to n-1
void setElement(Matrix *matrix, const int rowNumber, const int columnNumber, double value)
{
    const int index = calculateIndex(matrix, rowNumber, columnNumber);
    matrix->data[index] = value;
}

void fprintMatrix(FILE *file, const Matrix *matrix)
{
    for (int row = 0; row < matrix->rowCount; row++)
    {
        for (int column = 0; column < matrix->columnCount; column++)
        {
            const double value = getElement(matrix, row, column);
            fprintf(file, "%6.2lf   ", value);
        }
        fprintf(file, "\n");
    }
}

void printMatrix(const Matrix *matrix)
{
    fprintMatrix(stdout, matrix);
}
