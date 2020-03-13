package pl.pw;

import lombok.val;
import org.javatuples.Pair;

import java.util.ArrayList;
import java.util.List;

public class Utils {
    /**
     * Divide single range into rangeCount parts. Each part takes form of [a, b)
     * Example: splitIntoRanges(10, 3) => [0, 4), [4, 8), [8, 10)
     */
    public static List<Pair<Integer, Integer>>  splitIntoRanges(final int size, final int rangeCount) {
        val ranges = new ArrayList<Pair<Integer, Integer>>(rangeCount);

        final int partSize = (int) Math.ceil((double)size / (double)rangeCount);
        for (int i = 0, currentIndex = 0; i < rangeCount; i++) {
            Pair<Integer, Integer> range;

            if (currentIndex < size) {
                range = Pair.with(currentIndex, Math.min(currentIndex + partSize, size));
                currentIndex = range.getValue1();
            }
            else {
                range = Pair.with(0, 0);
            }

            ranges.add(range);
        }

        return ranges;
    }
}
