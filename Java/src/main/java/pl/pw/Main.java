package pl.pw;

import lombok.AllArgsConstructor;
import lombok.val;
import lombok.var;
import org.javatuples.Pair;

import java.io.FileNotFoundException;
import java.util.ArrayList;

public class Main {

    @AllArgsConstructor
    public static class MultiplicationThread implements Runnable {
        private final Matrix matrixA;
        private final Matrix matrixB;
        private final Matrix resutlsMatrix;
        private final Pair<Integer, Integer> range;
        private static Object sumMutex;

        static {
            sumMutex = new Object();
        }


        @Override
        public void run() {
            for (int index = range.getValue0(); index < range.getValue1(); index++) {
                Pair<Integer, Integer> rowAndColumn = resutlsMatrix.mapIndexToRowAndColumn(index);
                final int row = rowAndColumn.getValue0();
                final int column = rowAndColumn.getValue1();

                double sum = 0.;
                for (int i = 0; i < matrixA.getColumnCount(); i++) {
                    val a = matrixA.getElement(row, i);
                    val b = matrixB.getElement(i, column);
                    sum += a.doubleValue() * b.doubleValue();
                }

                resutlsMatrix.setElement(row, column, sum);

                synchronized (sumMutex) {
                    elementSum += sum;
                }
            }
        }
    }

    public static double elementSum;
    public static Matrix<Double> multiplyAndSum(Matrix matrixA, Matrix matrixB, final int threadCount) throws InterruptedException {
        if (matrixA.getColumnCount() != matrixB.getRowCount())
            throw new IllegalArgumentException("Matrix dimensions doesn't match");

        val resultMatrix = Matrix.zeros(matrixA.getRowCount(), matrixB.getColumnCount());
        elementSum = 0.;

        final int maxIndex = resultMatrix.getRowCount() * resultMatrix.getColumnCount();
        val ranges = Utils.splitIntoRanges(maxIndex, threadCount);

        val threads = new ArrayList<Thread>(threadCount);
        for (int i = 0; i < threadCount; i++) {
            val thread = new Thread(new MultiplicationThread(matrixA, matrixB, resultMatrix, ranges.get(i)));
            thread.start();
            threads.add(thread);
        }

        for (Thread thread : threads) {
            thread.join();
        }

        return resultMatrix;
    }

    public static void main(String[] args) {
        var test = "Hi";
        System.out.println("Hi Gradle. Lest's do matrix multiplication");

        try {
            val matrixA = Matrix.loadFromFile("A.txt");
            val matrixB = Matrix.loadFromFile("B.txt");

            val matrixC = multiplyAndSum(matrixA, matrixB, 3);

            System.out.println(matrixC);
            System.out.println(elementSum);
        } catch (Matrix.InvalidFileFormatException | FileNotFoundException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
