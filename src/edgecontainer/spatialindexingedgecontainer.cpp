#include "spatialindexingedgecontainer.h"

using namespace std;

SpatialIndexingEdgeContainer::SpatialIndexingEdgeContainer(double xSize, double ySize, int m, int n) :
    mXSize(xSize),
    mYSize(ySize),
    mM(m),
    mN(n),
    mXStep(xSize / m),
    mYStep(ySize / n),
    mChildGrids(m, std::vector<SpatialIndexingEdgeContainer*>(n) ),
    mContainedEdges(NULL)
{

}

SpatialIndexingEdgeContainer::~SpatialIndexingEdgeContainer()
{

}

//list<list<Edge>*> SpatialIndexingEdgeContainer::findNeighbourEdges(const Edge& edge) const
//{

//}

list<Edge>* SpatialIndexingEdgeContainer::containedEdges()
{
    return new list<Edge>;
}
