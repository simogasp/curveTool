#include "approximation.h"

Point deCasteljau(std::vector<Point> controlPoints, std::size_t start, std::size_t end, double t)
{
    // lerping n (n - 1) / 2 times
    for(std::size_t i = end; i > start; --i)
    {
        for(std::size_t j = start; j < i; ++j)
            controlPoints[j] = lerp(controlPoints[j], controlPoints[j + 1], t);
    }
    return controlPoints[start];
}