#pragma once

#include <stdlib.h>
#include <math.h>
#include <sys/param.h>
#include "pair.h"

// Divide single range into rangeCount parts. Each part takes form of [a, b)
// Example: splitIntoRanges(10, 3) => [0, 4), [4, 8), [8, 10)
Pair *splitIntoRanges(const int vectorSize, const int rangeCount);
