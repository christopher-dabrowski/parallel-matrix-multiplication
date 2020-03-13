package pl.pw;

import lombok.val;
import org.javatuples.Pair;
import org.junit.Test;

import java.util.Arrays;

import static org.junit.Assert.*;

public class UtilsTest {

    @Test
    public void splitIntoRanges() {
        // Given
        final int vectorSize = 10;
        final int threadCount = 3;

        // When
        val ranges = Utils.splitIntoRanges(vectorSize, threadCount);

        // Then
        val expectedSplit = Arrays.asList(Pair.with(0, 4), Pair.with(4, 8), Pair.with(8, 10));
        assertEquals(expectedSplit, ranges);
    }
}