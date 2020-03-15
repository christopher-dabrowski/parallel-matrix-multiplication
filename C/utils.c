#include "utils.h"

Pair *splitIntoRanges(const int vectorSize, const int rangeCount)
{
    Pair *ranges = malloc(rangeCount * sizeof(*ranges));

    const int partSize = (int)ceil((double)vectorSize / (double)rangeCount);
    for (int i = 0, current = 0; i < rangeCount; i++)
    {
        if (current < vectorSize)
        {
            ranges[i].a = current;
            ranges[i].b = MIN(current + partSize, vectorSize);

            current = ranges[i].b;
        }
        else // There are more parts than size
        {
            ranges[i].a = 0;
            ranges[i].b = 0;
        }
    }

    return ranges;
}
