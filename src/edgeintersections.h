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

    EdgeIntersections();
    EdgeIntersections(const Edge* edge, const std::list<Intersection>& intersections);

    std::auto_ptr<const std::list<Intersection> > intersections() const;
    void addIntersection(const Intersection& intersection);
};

#endif // EDGEINTERSECTIONS_H
