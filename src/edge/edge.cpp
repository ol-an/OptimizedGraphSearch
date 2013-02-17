#include "edge.h"

#include <iostream>
#include "../geometry/geometryhelper.h"

using namespace std;

double Edge::eps = 1.5;

Edge::Edge() : mVertices(-1, -1), mIsFirstPointPullable(true), mIsLastPointPullable(true)
{

}

Edge::Edge(int v1, int v2, const std::vector<Point>& points) : mVertices(v1, v2), vector<Point>(points), mIsFirstPointPullable(true), mIsLastPointPullable(true)
{
    updateBoundingBox();
}

/**
 * @brief ������������ ����������� ����� ������ this � other.
 *
 * ����������� ���������� ��� � ���������� ����������������� ����������� ��������, �� ������� ������� ������� *this � other,
 * ��� � � ���������� ����������� ��������� ����� ����� *this � other �� ��������� ���� ����� (������������ ����� pullUpEndingPointsToEdge).
 *
 * ��� ������� ����������� � ���������� ����������� ��������� ����� ������������ ����� pointsToSkip ����� *this, �.�.
 * ��� �� �����, ������� ��� ������ ���� ���� �������� �� ��������� ����� ������ ����� � ���������� ������ ������
 * appendEndingPoints.
 *
 * ��������� ����� ����������� ����� this � ������ ownIntersections.
 * ��������� - O(n^2) ��� �����������, ���� bounding box-� �� ������������.
 *
 * @param[in] other �����, � ������� �������������� �����������.
 * @param[out] ownIntersections ������ ����������� ����� this, � ������� ����������� ����� �����������.
 * @param[inout] freeVertexNumber ��������� ����� ������� �����, � �������� ����� ���������� ��������� �����������.
 * @return ������ ����������� ����� other.
 */
auto_ptr<EdgeIntersections> Edge::calculateIntersectionsOfTwoEdges(Edge& other,
                                                                   size_t& freeVertexNumber,
                                                                   EdgeIntersections& ownIntersections)
{
    auto_ptr<EdgeIntersections> otherIntersections(new EdgeIntersections(&other));

    if(size() == 0 || other.size() == 0 || !mBoundingBox.intersects(other.mBoundingBox))
    {
        return otherIntersections;
    }

    pullUpEndingPointsToEdge(other, freeVertexNumber, *otherIntersections);
    other.pullUpEndingPointsToEdge(*this, freeVertexNumber, ownIntersections);

    const_iterator ownEnd = end() - 1;
    const_iterator otherEnd = other.end() - 1;
    Intersection ownIntersection, otherIntersection;
    Point lastIntersection(DBL_MAX, DBL_MAX);
    bool intersectedAtPreviousIteration = false;
    for(const_iterator ownVertex = begin(); ownVertex != ownEnd; ++ownVertex)
    {
        for(const_iterator otherVertex = other.begin(); otherVertex != otherEnd; ++otherVertex)
        {
            if(GeometryHelper::intersection(
                        Segment(*ownVertex, *(ownVertex + 1)),
                        Segment(*otherVertex, *(otherVertex + 1)),
                        GeometryHelper::LineTypeSegment,
                        GeometryHelper::LineTypeSegment,
                        true,
                        &ownIntersection.point))
            {
                if(intersectedAtPreviousIteration && (lastIntersection == ownIntersection.point))
                {
                    continue;
                }
                lastIntersection = ownIntersection.point;
                ownIntersection.segmentNumber = std::distance(static_cast<const_iterator>(begin()), ownVertex);
                ownIntersection.vertexNumber = freeVertexNumber++;
                otherIntersection = ownIntersection;
                otherIntersection.segmentNumber = std::distance(static_cast<const_iterator>(other.begin() ), otherVertex);

                ownIntersections.add(ownIntersection);
                otherIntersections->add(otherIntersection);

                intersectedAtPreviousIteration = true;
            }
            else
            {
                intersectedAtPreviousIteration = false;
            }
        }
    }

    return otherIntersections;
}

/**
 * @brief ������������ ��������� ����� ������� ����� � ��������� ������ ����� edges, ���� ��� ��������� � ������� eps.
 *
 * ��� ���� ����� ������� �������� ����� ���������� �� ����� ������� ���� �����, � �������� �������������� ������.
 *
 * @param edges �����, � ������� �� �������� ������������ ������.
 * @return ���������� � �������������� ������ (�� �����, ������, ��������� ��� ���).
 */

void Edge::appendEndingPoints(const list<Edge>& edges)
{
    if(size() == 0)
    {
        return;
    }

    int appendedPoints = 0;
    bool foundPointToAppend = false;

    for(int i = 0; i < 2; ++i)
    {
        iterator myPoint = i == 0 ? begin() : end() - 1;
        for(list<Edge>::const_iterator edge = edges.begin(); edge != edges.end(); ++edge)
        {
            if(edge->size() == 0)
            {
                continue;
            }
            for(int j = 0; j < 2; ++j)
            {
                const_iterator otherPoint = j == 0 ? edge->begin() : edge->end() - 1;
                if(GeometryHelper::distance(*myPoint, *otherPoint) < eps)
                {
                    *myPoint = *otherPoint;
                    size_t *myVertex, otherVertex;
                    if(i == 0)
                    {
                        myVertex = &mVertices.first;
                        appendedPoints |= 1;
                    }
                    else
                    {
                        myVertex = &mVertices.second;
                        appendedPoints |= 2;
                    }
                    otherVertex = j == 0 ? edge->mVertices.first : edge->mVertices.second;
                    *myVertex = otherVertex;
                    foundPointToAppend = true;
                    break;
                }
            }
            if(foundPointToAppend)
            {
                foundPointToAppend = false;
                break;
            }
        }
    }
    updateBoundingBox();
    if((appendedPoints & 1) != 0)
    {
        mIsFirstPointPullable = false;
    }
    if((appendedPoints & 2) != 0)
    {
        mIsLastPointPullable = false;
    }
}

void Edge::pullUpEndingPointsToEdge(const Edge &edge, size_t& freeVertexNumber, EdgeIntersections &intersections)
{
    if(size() == 0 || edge.size() == 0)
    {
        return;
    }
    const const_iterator last = edge.end() - 1;

    iterator pullingPoint;
    for(int i = 1; i < 3; ++i)
    {
        if((i == 1 && !mIsFirstPointPullable) || (i == 2 && !mIsLastPointPullable))
        {
            continue;
        }
        pullingPoint = i == 1 ? begin() : end() - 1;
        for(const_iterator it = edge.begin(); it != last; ++it)
        {
            if(GeometryHelper::pullUpPointToSegment(Segment(*it, *(it + 1)), eps, *pullingPoint))
            {
                intersections.add(Intersection(std::distance(edge.begin(), it), *pullingPoint, freeVertexNumber++));
                i == 1 ? mIsFirstPointPullable = false : mIsLastPointPullable = false;
                break;
            }
        }
    }
}

/**
 * @brief ���������� ������ ���� ����������� ����� *this � ������� edges.
 *
 * @note ����������� *this ����������� � ������ ����������!
 *
 * @param edges

 * @param freeVertexNumber
 * @return
 */
auto_ptr<list<EdgeIntersections> > Edge::calculateAllIntersections(
        std::list<Edge*>& edges,
        size_t& freeVertexNumber)
{
    auto_ptr<list<EdgeIntersections> > allIntersections(new list<EdgeIntersections>);

    EdgeIntersections ownIntersecions(this);
    for(list<Edge*>::iterator edge = edges.begin(); edge != edges.end(); ++edge)
    {
        auto_ptr<EdgeIntersections> otherIntersections = calculateIntersectionsOfTwoEdges(**edge, freeVertexNumber, ownIntersecions);
        if(otherIntersections->size() != 0)
        {
            allIntersections->push_back(*otherIntersections);
        }
    }
    allIntersections->push_back(ownIntersecions);
    return allIntersections;
}

/**
 * @brief ��������� ������ ����� �� ��������� ����� �� ������ ����������� intersections.
 *
 * @note ������ ������ ���� ������������!
 * @note ������ ����� � ������������ ������ ��������� � ������� new. � ����� ����� ������ ������� � ������� delete.
 *
 * @param intersections
 * @return
 */
auto_ptr<list<Edge*> > Edge::split(const EdgeIntersections& intersections) const
{
    if(this != intersections.edge)
    {
        cout << "Warning: split's argument is not an intersection list for current edge!\n";
    }

    auto_ptr<list<Edge*> > edges(new list<Edge*>);

    if(intersections.size() == 0 || size() < 2)
    {
        Edge* edge = new Edge(mVertices.first, mVertices.second, *this);
        edge->mIsFirstPointPullable = mIsFirstPointPullable;
        edge->mIsLastPointPullable = mIsLastPointPullable;
        edge->updateBoundingBox();
        edges->push_back(edge);
        return edges;
    }

    Edge* edge = new Edge;
    edge->push_back(front());
    edge->mVertices.first = mVertices.first;
    edge->mIsFirstPointPullable = mIsFirstPointPullable;
    const_iterator currentVertex = begin() + 1;
    list<Intersection>::const_iterator currentIntersection = intersections.intersections()->begin();
    size_t segmentNumber = 0;
    while(currentVertex != end())
    {
        if(currentIntersection != intersections.intersections()->end() && currentIntersection->segmentNumber == segmentNumber)
        {
            edge->push_back(currentIntersection->point);
            edge->mIsLastPointPullable = false;
            edge->mVertices.second = currentIntersection->vertexNumber;
            edge->updateBoundingBox();
            edges->push_back(edge);
            edge = new Edge;
            edge->push_back(currentIntersection->point);
            edge->mIsFirstPointPullable = false;
            edge->mVertices.first = currentIntersection->vertexNumber;
            ++currentIntersection;
        }
        else
        {
            edge->push_back(*(currentVertex++));
            ++segmentNumber;
        }
    }
    edge->mIsLastPointPullable = mIsLastPointPullable;
    edge->mVertices.second = mVertices.second;
    edges->push_back(edge);

    return edges;
}
