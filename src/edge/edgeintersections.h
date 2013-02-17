#ifndef EDGEINTERSECTIONS_H
#define EDGEINTERSECTIONS_H

#include <list>
#include <memory>

#include "intersection.h"

class Edge;

class EdgeIntersections
{
private:
    std::list<Intersection> mIntersections;

public:
    Edge* edge;

    EdgeIntersections(Edge* edge, const std::list<Intersection>& intersections = std::list<Intersection>());

    const std::list<Intersection>* intersections() const {return &mIntersections;}
    size_t size() const {return mIntersections.size();}
    void add(const Intersection& intersection);
};

#endif // EDGEINTERSECTIONS_H
