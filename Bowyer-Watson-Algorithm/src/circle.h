#ifndef CIRCLE_H
#define CIRCLE_H

#include <cmath>

#include "utilities.h"
#include "node.h"

class Circle
{
public:
    Circle() = default;
    Circle(Node center_, float radius_) : center(center_), radius(radius_) {}

    inline bool containsNode(const Node& n) const noexcept
    {
        return distance(this->center, n) < radius;
    }

public:
    Node center;
    float radius;
};

#endif // CIRCLE_H
