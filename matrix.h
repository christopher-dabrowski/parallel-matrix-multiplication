#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pair.h"

typedef struct Matrix
{
    int rowCount;
    int columnCount;
    double *data;
} Matrix;

Matrix *createMatrix(const int rowCount, const int columnCount, double data[]);

// Create matrix filled with 0
Matrix *createZerosMatrix(const int rowCount, const int columnCount);

Matrix *loadMatrixFromFile(const char *fileName);

// Get index of inner matrix array
int calculateIndex(const Matrix *matrix, const int rowNumber, const int columnNumber);

// Map inner matrix array back to row and column number
Pair mapIndexToRowAndColumn(const Matrix *matrix, const int index);

// Clean up resources allocacted for Matrix
void disposeMatrix(Matrix *matrix);

// Indexes are numbered from 0 to n-1
double getElement(const Matrix *matrix, const int rowNumber, const int columnNumber);

// Indexes are numbered from 0 to n-1
void setElement(Matrix *matrix, const int rowNumber, const int columnNumber, double value);

void fprintMatrix(FILE *file, const Matrix *matrix);

void printMatrix(const Matrix *matrix);
