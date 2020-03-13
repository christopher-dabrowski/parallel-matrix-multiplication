package pl.pw;

import lombok.val;
import org.javatuples.Pair;
import org.junit.Test;

import static org.junit.Assert.*;

public class MatrixTest {

    @Test
    public void calculateIndex() {
        // Given
        val matrix = new Matrix<Double>(2, 3, new Double[]{2., 1., 3., -1., 4., 0.});

        // When
        int index = matrix.calculateIndex(1, 1);

        // Then
        assertEquals(4, index);
    }

    @Test
    public void mapIndexToRowAndColumn() {
        // Given
        val matrix = new Matrix<Double>(2, 3, new Double[]{2., 1., 3., -1., 4., 0.});

        // When
        val rowAndColumn = matrix.mapIndexToRowAndColumn(4);
        final int row = rowAndColumn.getValue0();
        final int column = rowAndColumn.getValue1();

        // Then
        assertEquals(1, row);
        assertEquals(1, column);
    }
}