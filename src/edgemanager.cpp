#include "edgemanager.h"

using namespace std;

EdgeManager::EdgeManager(double xSize, double ySize, int m, int n) :
    mXSize(xSize),
    mYSize(ySize),
    mM(m),
    mN(n),
    mXStep(xSize / m),
    mYStep(ySize / n),
    mChildGrids(m, std::vector<EdgeManager*>(n) ),
    mContainedEdges(NULL)
{

}

EdgeManager::~EdgeManager()
{

}

list<list<Edge>*> EdgeManager::findNeighbourEdges(const Edge& edge) const
{
    return list<list<Edge>*>();
}

list<Edge>* EdgeManager::containedEdges()
{
    return new list<Edge>;
}
