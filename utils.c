#include "utils.h"

Pair *splitIntoRanges(const int size, const int rangeCount)
{
    Pair *ranges = malloc(rangeCount * sizeof(*ranges));

    const int partSize = (int)ceil((double)size / (double)rangeCount);
    for (int i = 0, current = 0; i < rangeCount; i++)
    {
        if (current < size)
        {
            ranges[i].a = current;
            ranges[i].b = MIN(current + partSize, size);

            current = ranges[i].b;
        }
        else // Therea are more parts than size
        {
            ranges[i].a = 0;
            ranges[i].b = 0;
        }
    }

    return ranges;
}
