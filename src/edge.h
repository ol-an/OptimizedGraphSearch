#ifndef EDGE_H
#define EDGE_H

#include <list>
#include <memory>
#include <vector>

#include "boundingbox.h"
#include "edgeintersections.h"
#include "intersection.h"
#include "point.h"

class Cell;
class Edge;

typedef std::pair<int, int> Vertices;

class Edge : public std::vector<Point>
{
    friend class Cell;
private:
    Vertices mVertices;
    BoundingBox mBoundingBox;
    std::list<Cell*> mOwningCells;

    Edge(const Edge&);

    std::auto_ptr<EdgeIntersections> calculateIntersectionsOfTwoEdges(
            const Edge& other,
            EdgeIntersections *ownIntersections,
            int& freeVertexNumber);
public:

    Edge();
    Edge(int v1, int v2, const std::vector<Point>& points);

    void updateBoundingBox() {mBoundingBox.update(*this);}
    const BoundingBox* boundingBox() {return &mBoundingBox;}

    std::auto_ptr<std::list<EdgeIntersections> > calculateAllIntersections(const std::list<Edge>& edges, int& freeVertexNumber);

};

#endif // EDGE_H
