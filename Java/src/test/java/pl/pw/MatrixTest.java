package pl.pw;

import lombok.val;
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
}