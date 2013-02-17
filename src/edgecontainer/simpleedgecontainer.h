#ifndef SIMPLEEDGECONTAINER_H
#define SIMPLEEDGECONTAINER_H

#include "iedgecontainer.h"


class SimpleEdgeContainer : public IEdgeContainer
{
private:
    std::list<Edge*> mEdges;
    size_t mFreeVertexNumber;

public:
    SimpleEdgeContainer();
    ~SimpleEdgeContainer();
    void add(const std::vector<Point> &road);
    const std::list<Edge*>* allEdges();
};

#endif // SIMPLEEDGECONTAINER_H
