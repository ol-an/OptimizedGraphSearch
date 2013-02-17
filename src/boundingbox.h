#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <cfloat>
#include <vector>
#include "point.h"

class BoundingBox
{
private:
    double mLeft;
    double mRight;
    double mUpper;
    double mLower;

public:
    BoundingBox() : mLeft(DBL_MAX), mRight(-DBL_MAX), mUpper(-DBL_MAX), mLower(DBL_MAX) {}
    double left() const {return mLeft;}
    double right() const {return mRight;}
    double upper() const {return mUpper;}
    double lower() const {return mLower;}
    void update(const std::vector<Point>& object)
    {
        mLeft = mLower = DBL_MAX;
        mRight = mUpper = -DBL_MAX;
        for(std::vector<Point>::const_iterator it = object.begin(); it != object.end(); ++it)
        {
            if(mLeft < it->x)   mLeft = it->x;
            if(mLower < it->y)  mLower = it->y;
            if(mRight > it->x)  mRight = it->x;
            if(mUpper > it->y)  mUpper = it->y;
        }
    }
    bool intersects(const BoundingBox& other) const
    {
        if(other.mRight < mLeft ||
           other.mLeft > mRight ||
           other.mUpper < mLower ||
           other.mLower > mUpper)
        {
            return false;
        }
        return true;
    }
};

#endif // BOUNDINGBOX_H
