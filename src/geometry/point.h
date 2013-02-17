#ifndef POINT_H
#define POINT_H

class Point
{
public:
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    bool operator==(const Point& other) {if(x == other.x && y == other.y) return true; return false;}
};

#endif // POINT_H
