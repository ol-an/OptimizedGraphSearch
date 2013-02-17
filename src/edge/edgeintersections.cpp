#include "edgeintersections.h"

#include <algorithm>

#include "edge.h"
#include "../geometry/geometryhelper.h"

using namespace std;

class IntersectionComparator
{
private:
    const Edge* mEdge;
public:
    IntersectionComparator(const Edge* edge) : mEdge(edge) {}
    bool operator()(const Intersection& first, const Intersection& second)
    {
        if(first.segmentNumber < second.segmentNumber)
        {
            return true;
        }
        else if(first.segmentNumber > second.segmentNumber)
        {
            return false;
        }
        Point segmentBeginning = (*mEdge)[second.segmentNumber];
        const double dist1 = GeometryHelper::distance(first.point, segmentBeginning), dist2 = GeometryHelper::distance(second.point, segmentBeginning);

        return dist1 < dist2;
    }
};

EdgeIntersections::EdgeIntersections(Edge *edge, const std::list<Intersection> &intersections) : edge(edge), mIntersections(intersections) {}

void EdgeIntersections::add(const Intersection& intersection)
{
    if(size() == 0)
    {
        mIntersections.push_back(intersection);
        return;
    }
    mIntersections.insert(upper_bound(mIntersections.begin(), mIntersections.end(), intersection, IntersectionComparator(edge)), intersection);
}

