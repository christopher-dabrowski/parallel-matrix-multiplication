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

    @Test
    public void getElement() {
        // Given
        val matrix = new Matrix<Double>(2, 3, new Double[]{2., 1., 3., -1., 4., 0.});

        // When
        double selected = matrix.getElement(0, 2);

        // Then
        assertEquals(3., selected, 1e-5);
    }

    @Test
    public void setElement() {
        // Given
        val matrix = new Matrix<Double>(2, 3, new Double[]{2., 1., 3., -1., 4., 0.});

        // When
        matrix.setElement(0, 1, 42.);

        // Then
        assertEquals(42., matrix.getElement(0, 1), 1e-5);
    }

    @Test
    public void toStringTest() {
        // Given
        val matrix = new Matrix<Double>(2, 3, new Double[]{2., 1., 3., -1., 4., 0.});

        // When
        val string = matrix.toString();

        // Then
        val expected = "  2.00     1.00     3.00   \n" +
                       " -1.00     4.00     0.00   \n";
        assertEquals(expected, string);


    }

}