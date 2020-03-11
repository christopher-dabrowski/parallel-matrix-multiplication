#include "matrix.h"

static int calculateIndex(const Matrix *matrix, const int rowNumber, const int columnNumber)
{
    return rowNumber * matrix->columnCount + columnNumber;
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
        perror("Nie udało się otworzyć pliku\n");
        exit(EXIT_FAILURE);
    }

    int rowCount, columnCount;
    if (fscanf(file, "%d", &rowCount) != 1)
    {
        fclose(file);
        perror("Nie udało się wczytać liczby wierszy\n");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", &columnCount) != 1)
    {
        fclose(file);
        perror("Nie udało się wczytać liczby kolumn\n");
        exit(EXIT_FAILURE);
    }

    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rowCount = rowCount;
    matrix->columnCount = columnCount;
    matrix->data = malloc(rowCount * columnCount * sizeof(*matrix->data));

    for (int column = 0; column < columnCount; column++)
    {
        for (int row = 0; row < rowCount; row++)
        {
            const int index = calculateIndex(matrix, row, column);
            if (fscanf(file, "%lf", &matrix->data[index]) != 1)
            {
                fclose(file);
                free(matrix->data);
                fprintf(stderr, "Nie udało się wczytać elementu macierzy [%d, %d]\n", row + 1, column + 1);
                exit(EXIT_FAILURE);
            }
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
    for (int column = 0; column < matrix->columnCount; column++)
    {
        for (int row = 0; row < matrix->rowCount; row++)
        {
            const double value = getElement(matrix, row, column);
            fprintf(file, "%.4lf   ", value);
        }
        fprintf(file, "\n");
    }
}

void printMatrix(const Matrix *matrix)
{
    fprintMatrix(stdout, matrix);
}
