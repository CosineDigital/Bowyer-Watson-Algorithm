#ifndef NODE_H
#define NODE_H

class Node
{
public:
    Node() = default;
    Node(float x_, float y_) : x(x_), y(y_) {}

    inline friend bool operator == (const Node& p, const Node& q) noexcept
    {
        return (p.x == q.x) && (p.y == q.y);
    }

    inline friend bool operator != (const Node& p, const Node& q) noexcept
    {
        return (p.x != q.x) || (p.y != q.y);
    }

public:
    float x, y;
};

#endif // NODE_H
