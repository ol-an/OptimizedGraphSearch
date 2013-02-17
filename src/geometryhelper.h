#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

#include <vector>

#include "point.h"

typedef std::pair<Point, Point> Segment;

class GeometryHelper
{
private:
    static bool isStraightEnough(const std::vector<Point>& polyline, const double epsilon);
    static Point approximatePiece(std::vector<Point>::const_iterator& currentVertice, const std::vector<Point>::const_iterator endOfPolyline, const double epsilon);
    static const int minPieceSize = 2;

public:
    enum SegmentPosition
    {
        SegmentPositionInside,
        SegmentPositionOutside,
        SegmentPositionIntersects
    };

    enum LineType
    {
        LineTypeSegment,
        LineTypeStraightLine,
        LineTypeRay
    };
    static bool projection(const Point& point, const Segment& line, LineType type, Point* projectedPoint = NULL);
    static double distance(const Point& p1, const Point& p2);
    static double distance(const Point point, const Segment LineTypeSegment);
    static bool intersection(const Segment& first,
                             const Segment& second,
                             LineType firstType,
                             LineType secondType,
                             bool strictCompairson = false,
                             Point* intersection = NULL);
    static bool contains(const std::vector<Point>& polygon, const Point& point);
    static bool contains(const Point& bottomLeft, const Point& topRight, const Point& point);
    static Segment shortestBridge(const Segment& segment1, const Segment& segment2);
    static SegmentPosition segmentPolygonIntersection(const Segment& LineTypeSegment, const std::vector<Point>& polygon, Point* intersection = NULL);

    static SegmentPosition segmentPolygonIntersection(const Segment& LineTypeSegment, const std::vector<std::vector<Point> >& polygons, Point* intersection = NULL);

    static std::vector<Point> approximatePolyline(const std::vector<Point> &polyline, const double epsilon);
};

#endif // GEOMETRYHELPER_H
