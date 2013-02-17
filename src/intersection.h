#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "point.h"

class Intersection
{
public:
    int segmentNumber;
    Point point;
    int verticeNumber;

    Intersection() : segmentNumber(-1), verticeNumber(-1) {}
    Intersection(int segmentNumber, Point point, int verticeNumber) : segmentNumber(segmentNumber), point(point), verticeNumber(verticeNumber) {}
};

#endif // INTERSECTION_H
