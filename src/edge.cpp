#include "edge.h"

#include "geometryhelper.h"

using namespace std;

Edge::Edge() : mVertices(-1, -1)
{

}

Edge::Edge(int v1, int v2, const std::vector<Point>& points) : mVertices(v1, v2), std::vector<Point>(points)
{
    mBoundingBox.update(points);
}

/**
 * @brief Рассчитывает пересечения между ребром this и other.
 *
 * Добавляет точки пересечения ребра this в список ownIntersections.
 * Сложность - O(n^2) или константная, если bounding box-ы не пересекаются.
 *
 * @param[in] other Ребро, с которым рассчитывается пересечение.
 * @param[out] ownIntersections Список пересечений ребра this, в который добавляются новые пересечения.
 * @param[inout] freeVertexNumber Свободный номер вершины графа, с которого будет начинаться нумерация пересечений.
 * @return Список пересечений ребра other.
 */
auto_ptr<EdgeIntersections> Edge::calculateIntersectionsOfTwoEdges(const Edge& other,
                                                                     EdgeIntersections* ownIntersections,
                                                                     int& freeVertexNumber)
{
    auto_ptr<EdgeIntersections> otherIntersections(new EdgeIntersections);

    if(size() == 0 || other.size() == 0 || !mBoundingBox.intersects(other.mBoundingBox))
    {
        return otherIntersections;
    }

    const_iterator ownEnd = end() - 1;
    const_iterator otherEnd = other.end() - 1;
    Intersection ownIntersection, otherIntersection;
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
                ownIntersection.segmentNumber = std::distance(static_cast<const_iterator>(begin()), ownVertex);
                ownIntersection.verticeNumber = freeVertexNumber++;
                otherIntersection = ownIntersection;
                otherIntersection.segmentNumber = std::distance(static_cast<const_iterator>(other.begin() ), otherVertex);

                ownIntersections->addIntersection(ownIntersection);
                otherIntersections->addIntersection(otherIntersection);
            }
        }
    }

    return otherIntersections;
}


