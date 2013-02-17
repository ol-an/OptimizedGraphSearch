#ifndef EDGEMANAGER_H
#define EDGEMANAGER_H

#include <list>
#include <vector>

#include "edge.h"

class EdgeManager
{
private:
    double mXSize;
    double mYSize;
    int mM;
    int mN;
    double mXStep;
    double mYStep;
    std::vector<std::vector<EdgeManager*> > mChildGrids;
    std::list<Edge*>* mContainedEdges;

public:
    EdgeManager(double xSize, double ySize, int m, int n);
    ~EdgeManager();

    std::list<std::list<Edge>*> findNeighbourEdges(const Edge& edge) const;
    std::list<Edge>* containedEdges();

};

#endif // EDGEMANAGER_H
