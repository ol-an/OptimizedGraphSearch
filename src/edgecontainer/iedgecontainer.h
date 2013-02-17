#ifndef IEDGECONTAINER_H
#define IEDGECONTAINER_H

#include <list>
#include <vector>

#include "../edge/edge.h"

class IEdgeContainer
{
protected:
    virtual void erase(Edge* edge)
    {
        for(std::list<Occurence>::iterator it = edge->mOccurencies.begin(); it != edge->mOccurencies.end(); ++it)
        {
            it->first->erase(it->second);
        }
        delete edge;
    }

public:
    virtual ~IEdgeContainer() {}
    virtual void add(const std::vector<Point>& road) = 0;
    virtual const std::list<Edge*>* allEdges() = 0;
};

#endif // IEDGECONTAINER_H
