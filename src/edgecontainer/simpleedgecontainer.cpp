#include "simpleedgecontainer.h"

using namespace std;

SimpleEdgeContainer::SimpleEdgeContainer() : mFreeVertexNumber(0)
{
}

SimpleEdgeContainer::~SimpleEdgeContainer()
{
    for(list<Edge*>::iterator it = mEdges.begin(); it != mEdges.end(); ++it)
    {
        delete *it;
    }
}

void SimpleEdgeContainer::add(const vector<Point>& road)
{
    Edge edge(mFreeVertexNumber++, mFreeVertexNumber++, road);
    auto_ptr<list<EdgeIntersections> > intersections = edge.calculateAllIntersections(mEdges, mFreeVertexNumber);
    for(list<EdgeIntersections>::iterator edgeIntersection = intersections->begin(); edgeIntersection != intersections->end(); ++edgeIntersection)
    {
        auto_ptr<list<Edge*> > splitted = edgeIntersection->edge->split(*edgeIntersection);
        for(list<Edge*>::iterator it = splitted->begin(); it != splitted->end(); ++it)
        {
            mEdges.push_back(*it);
            Occurence o(&mEdges, mEdges.end());
            --o.second;
            (*it)->mOccurencies.push_back(o);
        }
    }
}

const std::list<Edge *> *SimpleEdgeContainer::allEdges()
{
    return &mEdges;
}
