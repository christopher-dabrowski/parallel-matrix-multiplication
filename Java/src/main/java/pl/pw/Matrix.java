package pl.pw;

import lombok.Getter;
import lombok.val;
import org.javatuples.Pair;

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

    // Row and column numbers start from 0
    public int calculateIndex(final int row, final int column)
    {
        return row * this.columnCount + column;
    }

    public Pair<Integer, Integer> mapIndexToRowAndColumn(final int index) {
        return Pair.with(index / this.columnCount, index % this.columnCount);
    }

//    public T getElement(final int row, final int columnCount) {
////        return
//    }

//    @Override
//    public String toString()
//    {
//        val resultBuilder = new StringBuilder();
//
//        for (int row = 0; row < this.rowCount; row++) {
//            for (int column = 0; column < columnCount; column++) {
////                resultBuilder.append()
//            }
//        }
//
//        return "WIP";
//    }
}
