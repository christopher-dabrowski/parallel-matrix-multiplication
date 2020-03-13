package pl.pw;

import lombok.Cleanup;
import lombok.Getter;
import lombok.val;
import org.javatuples.Pair;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.IllegalFormatException;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Matrix<T extends Number>  {
    @Getter
    private int rowCount;

    @Getter
    private int columnCount;

    private T data[];

    public Matrix(final int rowCount, final int columnCount, final T[] data) {
        if (data.length != rowCount * columnCount)
            throw new IllegalArgumentException("Invalid data lenght");

        this.rowCount = rowCount;
        this.columnCount = columnCount;
        this.data = data;
    }

    public static Matrix<Double> zeros(final int rowCount, final int columnCount) {
        return new Matrix<Double>(rowCount, columnCount, new Double[rowCount * columnCount]);
    }

    public static Matrix<Double> loadFromFile(final String fileName) throws FileNotFoundException, InvalidFileFormatException {
        val file = new File(fileName);
        @Cleanup val scanner = new Scanner(file);

        try {
            final int rowCount = scanner.nextInt();
            final int columnCount = scanner.nextInt();
            val matrix = new Matrix<Double>(rowCount, columnCount, new Double[rowCount * columnCount]);

            for (int row = 0; row < rowCount; row++) {
                for (int column = 0; column < columnCount; column++) {
                    matrix.data[matrix.calculateIndex(row, column)] = scanner.nextDouble();
                }
            }

            return matrix;
        } catch (NoSuchElementException e) {
            throw new InvalidFileFormatException("Matrix file format is invalid");
        }
    }

    // Row and column numbers start from 0
    public int calculateIndex(final int row, final int column)
    {
        return row * this.columnCount + column;
    }

    public Pair<Integer, Integer> mapIndexToRowAndColumn(final int index) {
        return Pair.with(index / this.columnCount, index % this.columnCount);
    }

    public T getElement(final int row, final int column) {
        return data[calculateIndex(row, column)];
    }

    public void setElement(final int row, final int column, T value) {
        data[calculateIndex(row, column)] = value;
    }

    @Override
    public String toString()
    {
        val resultBuilder = new StringBuilder();

        for (int row = 0; row < this.rowCount; row++) {
            for (int column = 0; column < columnCount; column++) {
                double value = this.data[calculateIndex(row, column)].doubleValue();
                resultBuilder.append(String.format("%6.2f   ", value));
            }
            resultBuilder.append('\n');
        }

        return resultBuilder.toString();
    }

    public static class InvalidFileFormatException extends Exception {
        public InvalidFileFormatException(String errorMessage) {
            super(errorMessage);
        }
    }
}
