package pl.pw;

import lombok.AllArgsConstructor;
import lombok.val;
import lombok.var;
import org.apache.commons.lang3.mutable.MutableDouble;
import org.javatuples.Pair;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.stream.DoubleStream;

public class Main {
    // Classes directly connected with the task are moved here, knowingly breaking OOP concepts

    @AllArgsConstructor
    public static class MultiplicationThread implements Runnable {
        private final Matrix matrixA;
        private final Matrix matrixB;
        private final Matrix resultsMatrix;
        private final Pair<Integer, Integer> range;
        private final MutableDouble sumOut;

        private static Object sumMutex;

        static {
            sumMutex = new Object();
        }

        @Override
        public void run() {
            for (int index = range.getValue0(); index < range.getValue1(); index++) {
                Pair<Integer, Integer> rowAndColumn = resultsMatrix.mapIndexToRowAndColumn(index);
                final int row = rowAndColumn.getValue0();
                final int column = rowAndColumn.getValue1();

                double sum = 0.;
                for (int i = 0; i < matrixA.getColumnCount(); i++) {
                    val a = matrixA.getElement(row, i);
                    val b = matrixB.getElement(i, column);
                    sum += a.doubleValue() * b.doubleValue();
                }

                resultsMatrix.setElement(row, column, sum);

                if (sumOut != null) {
                    synchronized (sumMutex) {
                        sumOut.add(sum);
                    }
                }
            }
        }
    }

    public static Matrix<Double> multiplyAndSum(Matrix matrixA, Matrix matrixB, final int threadCount,
                                                final MutableDouble elementsSumOut)
            throws InterruptedException {
        if (matrixA.getColumnCount() != matrixB.getRowCount())
            throw new IllegalArgumentException("Matrix dimensions doesn't match");

        val resultMatrix = Matrix.zeros(matrixA.getRowCount(), matrixB.getColumnCount());
        if (elementsSumOut != null)
            elementsSumOut.setValue(0.);

        final int maxIndex = resultMatrix.getRowCount() * resultMatrix.getColumnCount();
        val ranges = Utils.splitIntoRanges(maxIndex, threadCount);

        val threads = new ArrayList<Thread>(threadCount);
        for (int i = 0; i < threadCount; i++) {
            val thread = new Thread(new MultiplicationThread(matrixA, matrixB, resultMatrix, ranges.get(i), elementsSumOut));
            thread.start();
            threads.add(thread);
        }

        for (Thread thread : threads) {
            thread.join();
        }

        return resultMatrix;
    }

    @AllArgsConstructor
    public static class SquareAndSumThread implements Runnable {
        private final Matrix matrix;
        private final int threadId;
        private final Pair<Integer, Integer> range;
        private final double[] outputArray;

        @Override
        public void run() {
            double sum = 0.;
            for (int index = range.getValue0(); index < range.getValue1(); index++) {
                Pair<Integer, Integer> rowAndColumn = matrix.mapIndexToRowAndColumn(index);
                final int row = rowAndColumn.getValue0();
                final int column = rowAndColumn.getValue1();

                double value = matrix.getElement(row, column).doubleValue();
                sum += value * value;
            }

            outputArray[threadId] = sum;
        }

    }

    /**
     * Calculate Frobenius norm of given matrix
     *
     * Each thread will sum a part of the matrix and output result to shared list.
     */
    public static double frobeniusNorm(Matrix matrix, final int threadCount) throws InterruptedException {
        double[] partialSquareSums = new double[threadCount];

        final int maxIndex = matrix.getRowCount() * matrix.getColumnCount();
        val ranges = Utils.splitIntoRanges(maxIndex, threadCount);

        val threads = new ArrayList<Thread>(threadCount);
        for (int i = 0; i < threadCount; i++) {
            val thread = new Thread(new SquareAndSumThread(matrix, i, ranges.get(i), partialSquareSums));
            thread.run();
            threads.add(thread);
        }

        for (Thread thread : threads) {
            thread.join();
        }

        // Join partial sums and calculate final value
        return Math.sqrt(DoubleStream.of(partialSquareSums).sum());
    }

    public static int getThreadCount(String[] args)
    {
        if (args.length <= 0) {
            int cores = Runtime.getRuntime().availableProcessors();
            System.out.println(String.format("Nie podano ilości wątków. Program użyje domyślnej wartości: %d\n", cores));
            return cores;
        }

        try {
            int cores = Integer.parseInt(args[0]);
            if (cores <= 0) throw new NumberFormatException();

            return cores;
        } catch (NumberFormatException e) {
            System.err.println("Nieprawidłowa liczba wątków.\nLiczba wątków musi być dodatnią liczbą całkowitą");
            System.exit(-1);
        }

        return 1; //Line is never reached
    }

    public static void main(String[] args) throws InterruptedException {
        final int coreCount = getThreadCount(args);

        Matrix matrixA, matrixB;
        try {
            matrixA = Matrix.loadFromFile("A.txt");
            matrixB = Matrix.loadFromFile("B.txt");
        } catch (Matrix.InvalidFileFormatException | FileNotFoundException e) {
            System.err.println(e.getMessage());
            return;
        }

        System.out.println("Macierz A");
        System.out.println(matrixA);

        System.out.println("Macierz B");
        System.out.println(matrixB);

        val resultsSum = new MutableDouble();
        val matrixC = multiplyAndSum(matrixA, matrixB, coreCount, resultsSum);

        System.out.println("Iloczyn macierzy A * B");
        System.out.println(matrixC);

        System.out.println("Suma elementów macierzy C");
        System.out.println(resultsSum);

        System.out.println("\nNorma Frobeniusa macierzy A");
        System.out.println(frobeniusNorm(matrixA, coreCount));
    }
}
