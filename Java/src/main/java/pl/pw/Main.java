package pl.pw;

import lombok.AllArgsConstructor;
import lombok.val;
import lombok.var;
import org.apache.commons.lang3.mutable.MutableDouble;
import org.javatuples.Pair;

import java.io.FileNotFoundException;
import java.util.ArrayList;

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

    public static void main(String[] args) throws InterruptedException {
        try {
            val matrixA = Matrix.loadFromFile("A.txt");
            val matrixB = Matrix.loadFromFile("B.txt");

            val resultsSum = new MutableDouble();
            val matrixC = multiplyAndSum(matrixA, matrixB, 3, resultsSum);

            System.out.println(matrixC);
            System.out.println(resultsSum);
        } catch (Matrix.InvalidFileFormatException | FileNotFoundException e) {
            System.err.println(e.getMessage());
        }
    }
}
