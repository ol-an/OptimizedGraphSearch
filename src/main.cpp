#include <QCoreApplication>

#include <iostream>

#include "edge/edge.h"

using namespace std;

void test1();
void outputIntersection(const Intersection& i)
{
    cout << "point: " << i.point.x << '\t' << i.point.y << '\n';
    cout << "segment number: " << i.segmentNumber << '\n';
    cout << "v: " << i.vertexNumber << endl;
}
void outputEdgeIntersections(const EdgeIntersections& ei)
{
    cout << "edge: " << ei.edge << '\n';
    cout << "intersections count: " << ei.mIntersections.size() << '\n';
    auto_ptr<list<Intersection> > intersections = ei.intersections();
    for(list<Intersection>::iterator it = intersections->begin(); it != intersections->end(); ++it)
    {
        outputIntersection(*it);
    }
    cout << '\n';
}

void outputVectorP(const vector<Point>& v)
{
    for(vector<Point>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        cout << "x: " << it->x << '\t' << "y: " << it->y << '\n';
    }

}

void outputEdge(const Edge& e)
{
    cout << "Vertices: " << e.mVertices.first << ", " << e.mVertices.second << '\n';
    cout << "Pullabilites: " << e.mIsFirstPointPullable << ", " << e.mIsLastPointPullable << '\n';
    cout << "Points:\n";
    outputVectorP(e);
}

template<class T>
void append(const list<T>& appendWhat, list<T>& appendToWhat)
{
    list<T>::const_iterator it = appendWhat.begin();
    for(; it != appendWhat.end(); ++it)
    {
        appendToWhat.push_back(*it);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    test1();

    return a.exec();
}

void test1()
{
    cout << "test1()\n";

    vector<Point> myEdgePoints(5);
    myEdgePoints[0] = Point(1, 1);
    myEdgePoints[1] = Point(4, 5);
    myEdgePoints[2] = Point(12, 8);
    myEdgePoints[3] = Point(19, 4);
    myEdgePoints[4] = Point(26, 7);

    Edge myEdge(0, 1, myEdgePoints);
    cout << "MY EDGE : " << &myEdge << '\n';

    vector<Point> firstPoints(5);
    firstPoints[0] = Point(1, 6);
    firstPoints[1] = Point(1, 5);
    firstPoints[2] = Point(4, 2);
    firstPoints[3] = Point(7, 4);
    firstPoints[4] = Point(5, 8);

    Edge firstEdge(2, 3, firstPoints);
    cout << "FIRST EDGE : " << &firstEdge << '\n';

    vector<Point> secondPoints(5);
    secondPoints[0] = Point(17, 3);
    secondPoints[1] = Point(15, 1);
    secondPoints[2] = Point(13, 3);
    secondPoints[3] = Point(17, 9);
    secondPoints[4] = Point(16, 14);

    Edge secondEdge(4, 5, secondPoints);
    cout << "SECOND EDGE : " << &secondEdge << '\n';

    vector<Point> thirdPoints(4);
    thirdPoints[0] = Point(20, 15);
    thirdPoints[1] = Point(21, 12);
    thirdPoints[2] = Point(24, 14);
    thirdPoints[3] = Point(23, 16);

    Edge thirdEdge(6, 7, thirdPoints);
    cout << "THIRD EDGE : " << &thirdEdge << "\n\n\n";

    list<Edge*> edges;
    edges.push_back(&firstEdge);
    edges.push_back(&secondEdge);
    edges.push_back(&thirdEdge);

    size_t freeVertexNumber = 8;
    auto_ptr<list<EdgeIntersections> > intersections = myEdge.calculateAllIntersections(edges, freeVertexNumber);

    auto_ptr<list<Edge*> > newEdges(new list<Edge*>);//myEdge.split(intersections->back());

    for(list<EdgeIntersections>::iterator it = intersections->begin(); it != intersections->end(); ++it)
    {
        append(*it->edge->split(*it), *newEdges);
    }

    for(list<Edge*>::iterator it = newEdges->begin(); it != newEdges->end(); ++it)
    {
        outputEdge(**it); cout << '\n';
    }

    for(list<Edge*>::iterator it = newEdges->begin(); it != newEdges->end(); ++it)
    {
        delete *it;
    }
}
