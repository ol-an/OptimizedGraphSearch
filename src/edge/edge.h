#ifndef EDGE_H
#define EDGE_H

#include <list>
#include <memory>
#include <vector>

#include "../geometry/boundingbox.h"
#include "edgeintersections.h"
#include "intersection.h"
#include "../geometry/point.h"

typedef std::pair<size_t, size_t> Vertices;
typedef std::pair<std::list<Edge*>*, std::list<Edge*>::iterator> Occurence;

class IEdgeContainer;
class SimpleEdgeContainer;
class Edge : public std::vector<Point>
{
    friend void outputEdge(const Edge& e); //#TODO remove
    friend class IEdgeContainer;
    friend class SimpleEdgeContainer;
public:
    enum EndingPoints
    {
        EndingPointsNothing = 0,
        EndingPointsFirst = 1,
        EndingPointsLast = 2,
        EndingPointsBoth = 3
    };

private:
    Vertices mVertices;
    BoundingBox mBoundingBox;
    bool mIsFirstPointPullable;
    bool mIsLastPointPullable;

    std::list<Occurence> mOccurencies;


    Edge(const Edge&);

    std::auto_ptr<EdgeIntersections> calculateIntersectionsOfTwoEdges(Edge& other,
                                                                      size_t& freeVertexNumber,
                                                                      EdgeIntersections &ownIntersections);

    void pullUpEndingPointsToEdge(const Edge& edge, size_t &freeVertexNumber, EdgeIntersections &intersections);

public:

    Edge();
    Edge(int v1, int v2, const std::vector<Point>& points);

    void updateBoundingBox()
    {
        mBoundingBox.update(*this);
        mBoundingBox.mLeft -= eps;
        mBoundingBox.mLower -= eps;
        mBoundingBox.mUpper += eps;
        mBoundingBox.mRight += eps;
    }
    const BoundingBox* boundingBox() {return &mBoundingBox;}

    std::auto_ptr<std::list<EdgeIntersections> > calculateAllIntersections(
            std::list<Edge*> &edges,
            size_t& freeVertexNumber);

    void appendEndingPoints(const std::list<Edge>& edges);

    std::auto_ptr<std::list<Edge *> > split(const EdgeIntersections& intersections) const;

    static double eps;
};

#endif // EDGE_H
