#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <cstddef>

#include "../geometry/point.h"


class Intersection
{
public:
    size_t segmentNumber;
    Point point;
    size_t vertexNumber;

    Intersection() : segmentNumber(-1), vertexNumber(-1) {}
    Intersection(size_t segmentNumber, Point point, size_t verticeNumber) : segmentNumber(segmentNumber), point(point), vertexNumber(verticeNumber) {}
};

#endif // INTERSECTION_H
