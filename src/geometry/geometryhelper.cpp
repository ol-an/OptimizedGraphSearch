#include "geometryhelper.h"
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <float.h>

#include <iostream>



using namespace std;

bool operator==(const Point& left, const Point& right)
{
    if(left.x == right.x && left.y == right.y)
    {
        return true;
    }
    return false;
}

bool GeometryHelper::contains(const Point& bottomLeft, const Point& topRight, const Point& point)
{
    if(point.x > bottomLeft.x && point.x < topRight.x &&
       point.y > bottomLeft.y && point.y < topRight.y)
    {
	  return true;
    }
    return false;
}

bool GeometryHelper::intersection(const pair<Point, Point>& first,
                                  const pair<Point, Point>& second,
                                  GeometryHelper::LineType firstType,
                                  GeometryHelper::LineType secondType,
                                  bool strictCompairson,
                                  Point *intersection)
{
    //начальные точки прямых
    const double x1 = first.first.x;
    const double y1 = first.first.y;
    const double x2 = second.first.x;
    const double y2 = second.first.y;

    //коэффициенты параметрических уравнений прямых: x = x1 + mt, y = y1 + nt
    const double m1 = first.second.x - x1;
    const double m2 = second.second.x - x2;
    const double n1 = first.second.y - y1;
    const double n2 = second.second.y - y2;

    //если хоть один отрезок вырожден в точку - выходим
    if((m1 == 0 && n1 == 0) || (m2 == 0 && n2 == 0))
    {
        return false;
    }

    double denominator, t1, t2;

    if(fabs(m1) > fabs(n1))
    {
        denominator = n2 - n1 / m1 * m2;
        if(denominator == 0)    //прямые параллельны
        {
            return false;
        }
        t2 = (y1 - y2 + n1 / m1 * (x2 - x1) ) / denominator;
        t1 = (x2 - x1 + m2 * t2) / m1;
    }
    else
    {
        denominator = m2 - m1 / n1 * n2;
        if(denominator == 0)
        {
            return false;
        }
        t2 = (x1 - x2 + m1 / n1 * (y2 - y1) ) / denominator;
        t1 = (y2 - y1 + n2 * t2) / n1;
    }

    bool exceeds;
    if(!strictCompairson)
    {
        exceeds =
                (firstType == LineTypeSegment && (t1 < 0 || t1 > 1) ) ||
                (firstType == LineTypeRay && t1 < 0) ||
                (secondType == LineTypeSegment && (t2 < 0 || t2 > 1) ) ||
                (secondType == LineTypeRay && t2 < 0);
    }
    else
    {
        exceeds =
                (firstType == LineTypeSegment && (t1 <= 0 || t1 >= 1) ) ||
                (firstType == LineTypeRay && t1 <= 0) ||
                (secondType == LineTypeSegment && (t2 <= 0 || t2 >= 1) ) ||
                (secondType == LineTypeRay && t2 <= 0);
    }
    if(exceeds)
    {
        return false;
    }

    if(intersection != NULL)
    {
        intersection->x = x1 + t1 * m1;
        intersection->y = y1 + t1 * n1;
    }
    return true;
}

bool GeometryHelper::contains(const std::vector<Point> &polygon, const Point &point)
{
    assert(polygon.size() >= 3);

    int intersectionCount = 0;
    Point rightPoint(point);
    Point firstIntersection, previousIntersection, currentIntersection;
    bool intersectedFirst, intersectedPrevious, intersectedCurrent;

    rightPoint.x = DBL_MAX;

    const std::vector<Point>::const_iterator end = polygon.end() - 1;
    for(std::vector<Point>::const_iterator it = polygon.begin(); it != end; ++it)
    {
        if(intersectedCurrent = GeometryHelper::intersection(make_pair(*it, *(it + 1)), make_pair(point,  rightPoint), LineTypeSegment, LineTypeRay, false, &currentIntersection) )
        {
            ++intersectionCount;
        }
        if(it == polygon.begin())
        {
            intersectedFirst = intersectedCurrent;
            firstIntersection = currentIntersection;
        }
        else
        {
            if(intersectedPrevious && intersectedCurrent && (previousIntersection == currentIntersection))
            {
                if(((it - 1)->y - currentIntersection.y) * ((it + 1)->y - currentIntersection.y) < 0) //не выступ
                {
                    --intersectionCount;
                }
            }
        }
        intersectedPrevious = intersectedCurrent;
        previousIntersection = currentIntersection;
    }
    if(GeometryHelper::intersection(make_pair(*(polygon.end() - 1), *polygon.begin() ), make_pair(point, rightPoint), LineTypeSegment, LineTypeRay, false, &currentIntersection) )
    {
        ++intersectionCount;
        if(intersectedPrevious && intersectedFirst && (firstIntersection == previousIntersection))
        {
            if((polygon[polygon.size() - 1].y - firstIntersection.y) * (polygon[0].y - firstIntersection.y) < 0) //не выступ
            {
                --intersectionCount;
            }
        }
    }
    return ((intersectionCount % 2 == 1) ? true : false);
}

double GeometryHelper::distance(const Point &p1, const Point &p2)
{
    const double dx = p2.x - p1.x;
    const double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double GeometryHelper::distance(const Point point, const Segment segment)
{
    Point projected;
    projection(point, segment, LineTypeStraightLine, &projected);
    return distance(point, projected);
}

bool GeometryHelper::projection(const Point &point, const pair<Point, Point> &line, GeometryHelper::LineType type, Point* projectedPoint)
{
    const double m2 = line.second.x - line.first.x;
    const double n2 = line.second.y - line.first.y;
    const double dist = distance(line.first, line.second);
    Point secondPoint;

    double aspectRatio; //x / y

    if(fabs(m2) > fabs(n2))
    {
        aspectRatio = -n2 / m2;
        secondPoint.y = point.y + dist;
        secondPoint.x = point.x + aspectRatio * dist;
    }
    else
    {
        aspectRatio = -m2 / n2;
        secondPoint.x = point.x + dist;
        secondPoint.y = point.y + aspectRatio * dist;
    }

    return intersection(make_pair(point, secondPoint), line, LineTypeStraightLine, type, false, projectedPoint);
}

pair<Point, Point> GeometryHelper::shortestBridge(const pair<Point, Point> &segment1, const pair<Point, Point> &segment2)
{
    Point ab[2] = {segment1.first, segment1.second};
    Point cd[2] = {segment2.first, segment2.second};
    pair<Point, Point> bridges[8]; //a_c, a_d, b_c, b_d, a_cd, b_cd, c_ab, d_ab
    double distances[8];
    for(int i = 0; i < 2; ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            int index = 2 * i + j;
            bridges[index] = make_pair(ab[i], cd[j]);
            distances[index] = distance(bridges[index].first, bridges[index].second);
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        Point* currentSegment;
        pair<Point, Point> anotherSegment;
        if(i / 2 == 0)
        {
            currentSegment = ab;
            anotherSegment = segment2;
        }
        else
        {
            currentSegment = cd;
            anotherSegment = segment1;
        }
        Point projected;
        int index = 4 + i;
        if(projection(currentSegment[i % 2], anotherSegment, LineTypeSegment, &projected))
        {
            bridges[index] = make_pair(currentSegment[i % 2], projected);
            distances[index] = distance(bridges[index].first, bridges[index].second);
        }
        else
        {
            distances[index] = DBL_MAX;
        }
    }

    return bridges[std::distance(distances, min_element(distances, distances + 8) ) ];
}

GeometryHelper::SegmentPosition GeometryHelper::segmentPolygonIntersection(const pair<Point, Point> &segment, const std::vector<Point> &polygon, Point *intersection)
{
    std::vector<Point> roundedPolygon(polygon);
    roundedPolygon.push_back(polygon[0]);

    const std::vector<Point>::const_iterator end = roundedPolygon.end() - 1;
    double minDistance = DBL_MAX;
    bool intersects = false;
    Point closestVertex;
    for(std::vector<Point>::const_iterator it = roundedPolygon.begin(); it != end; ++it)
    {
        if(GeometryHelper::intersection(segment, make_pair(*it, *(it + 1)), LineTypeSegment, LineTypeSegment, false, intersection))
        {
            const double distance = GeometryHelper::distance(segment.first, *intersection);
            if(minDistance > distance)
            {
                intersects = true;
                minDistance = distance;
                closestVertex = *intersection;
            }
        }
    }
    if(intersects)
    {
        *intersection = closestVertex;
        return SegmentPositionIntersects;
    }
    if(contains(polygon, segment.first))
    {
        return SegmentPositionInside;
    }

    return SegmentPositionOutside;
}

GeometryHelper::SegmentPosition GeometryHelper::segmentPolygonIntersection(const pair<Point, Point>& segment, const std::vector<std::vector<Point> >& polygons, Point* intersection)
{
    Point localIntersection;
    SegmentPosition position;
    for(std::vector<std::vector<Point> >::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
    {
        position = segmentPolygonIntersection(segment, *it, &localIntersection);
        if(position == SegmentPositionIntersects)
        {
            *intersection = localIntersection;
            return SegmentPositionIntersects;
        }
    }
    return position;
}

std::vector<Point> GeometryHelper::approximatePolyline(const std::vector<Point>& polyline, const double epsilon)
{
    if(polyline.size() < 3)
    {
        return polyline;
    }

    std::vector<Point> approximated(1);
    approximated[0] = polyline.front();
    std::vector<Point>::const_iterator currentVertice = polyline.begin();
    const std::vector<Point>::const_iterator lastVertice = polyline.end() - 1;

    while(currentVertice != lastVertice)
    {
        approximated.push_back(approximatePiece(currentVertice, polyline.end(), epsilon) );
    }

    return approximated;
}

Point GeometryHelper::approximatePiece(std::vector<Point>::const_iterator& currentVertice, const std::vector<Point>::const_iterator endOfPolyline, const double epsilon)
{
    std::vector<Point> piece;
    for(int i = 0; currentVertice != endOfPolyline && i < minPieceSize; ++currentVertice, ++i)
    {
        piece.push_back(*currentVertice);
    }
    for(; currentVertice != endOfPolyline; ++currentVertice)
    {
        std::vector<Point> longerPiece(piece);
        longerPiece.push_back(*currentVertice);
        if(!isStraightEnough(longerPiece, epsilon))
        {
            break;
        }
        piece.push_back(*currentVertice);
    }
    return *(--currentVertice);
}

bool GeometryHelper::isStraightEnough(const std::vector<Point>& polyline, const double epsilon)
{

    if(polyline.size() <= 2)
    {
        return true;
    }
    const std::vector<Point>::const_iterator lastVertice = polyline.end() - 1;
    for(std::vector<Point>::const_iterator it = polyline.begin() + 1; it != lastVertice; ++it)
    {
        if(distance(*it, make_pair(polyline.front(), polyline.back() ) ) > epsilon)
        {
            return false;
        }
    }
    return true;
}

bool GeometryHelper::pullUpPointToSegment(const Segment& segment, const double eps, Point& point)
{
    if(distance(point, segment.first) < eps)
    {
        point = segment.first;
        return true;
    }
    if(distance(point, segment.second) < eps)
    {
        point = segment.second;
        return true;
    }
    Point proj;
    if(projection(point, segment, LineTypeSegment, &proj) )
    {
        if(distance(point, proj) < eps)
        {
            point = proj;
            return true;
        }
    }
    return false;
}
