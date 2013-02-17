#ifndef SPATIALINDEXINGEDGECONTAINER_H
#define SPATIALINDEXINGEDGECONTAINER_H

#include <list>
#include <vector>

#include "../edge/edge.h"
#include "iedgecontainer.h"

class SpatialIndexingEdgeContainer : public IEdgeContainer
{
private:
    double mXSize;
    double mYSize;
    int mM;
    int mN;
    double mXStep;
    double mYStep;
    std::vector<std::vector<SpatialIndexingEdgeContainer*> > mChildGrids;
    std::list<Edge*>* mContainedEdges;

public:
    SpatialIndexingEdgeContainer(double xSize, double ySize, int m, int n);
    ~SpatialIndexingEdgeContainer();

    std::list<std::list<Edge>*> findNeighbourEdges(const Edge& edge) const;
    std::list<Edge>* containedEdges();

};

#endif // SPATIALINDEXINGEDGECONTAINER_H
