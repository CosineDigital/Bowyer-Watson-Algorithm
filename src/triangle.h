#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <cmath>
#include <array>

#include "utilities.h"
#include "circle.h"
#include "node.h"
#include "segment.h"

class Triangle
{
public:
    Node p, q, r;
    std::array<Node, 3> nodesArray;
    std::array<Segment, 3> segArray;

public:
    Triangle() = default;
    /// constructor with 3 nodes
    Triangle(Node p_, Node q_, Node r_) : p(p_), q(q_), r(r_), circumCircle(this->getCircumCircle(p, q, r))
    {
        this->nodesArray = {p_, q_, r_};
        this->segArray = {Segment(p_, q_), Segment(q_, r_), Segment(r_, p_)};
    }

    /// constructor with a segment and a node, remove
    Triangle(const Segment& s, const Node& n) : p(s.p), q(s.q), r(n), circumCircle(this->getCircumCircle(s.p, s.q, n))
    {
        this->nodesArray = {s.p, s.q, n};
        this->segArray = {Segment(s.p, s.q), Segment(s.q, n), Segment(n, s.p)};
    }

    // getters, setters, and node checkers ...
    inline bool isNodeInCircumCircle(const Node& n) const noexcept
    {
        return this->circumCircle.containsNode(n);
    }

    inline void setIsBad(const bool& newValue) noexcept
    {
        this->isBad = newValue;
    }

    inline bool getIsBad() const noexcept
    {
        return this->isBad;
    }

    // operators ...
    friend bool operator == (const Triangle& tri1, const Triangle& tri2) noexcept
    {
        return (tri1.p == tri2.p) && (tri1.q == tri2.q) && (tri1.r == tri2.r);
    }

    friend bool operator != (const Triangle tri1, const Triangle tri2) noexcept
    {
        return tri1.p != tri2.p || tri1.q != tri2.q || tri1.r != tri2.r;
    }

    // circumcircle functions ...
    bool containsNodesOf(const Triangle& tri) const noexcept
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(this->nodesArray.at(i) == tri.nodesArray.at(j))
                    return true;
            }
        }
        return false;
    }

private:
    Circle circumCircle;
    bool isBad = false;

    /// TODO: still some bugs with this ...
    Circle getCircumCircle(const Node& n1, const Node& n2, const Node& n3) const noexcept
    {
        Node circumCenter = this->getIntersection(n1, n2, n3);
        return Circle(circumCenter, distance(circumCenter, n1));
    }

    Node getIntersection(const Node& n1, const Node& n2, const Node& n3) const noexcept
    {
        float top, bottom;
        // Nodes P and Q ============
        Segment segN1N2 = Segment(n1, n2);
        top    = n2.y - n1.y;
        bottom = n2.x - n1.x;

        if( std::abs(bottom) == 0.0f ) // ~/0 = infinite slope
            segN1N2.setSlopeType(SlopeType::INFINITE_SLOPE);
        else if(std::abs(top) == 0.0f) // 0/~ = zero slope
            segN1N2.setSlopeType(SlopeType::ZERO_SLOPE);
        else                           // ~/~ = normal slope
            segN1N2.setSlopeType(SlopeType::NORMAL_SLOPE);

        // Nodes Q and R ============
        Segment segN2N3 = Segment(n2, n3);
        top    = n3.y - n2.y;
        bottom = n3.x - n2.x;

        if( std::abs(bottom) == 0.0f ) // ~/0 = infinite slope
            segN2N3.setSlopeType(SlopeType::INFINITE_SLOPE);
        else if(std::abs(top) == 0.0f) // 0/~ = zero slope
            segN2N3.setSlopeType(SlopeType::ZERO_SLOPE);
        else                           // ~/~ = normal slope
            segN2N3.setSlopeType(SlopeType::NORMAL_SLOPE);

        // ==========================
        Node intersection;

        /// ====================================================
        // completely normal triangle
        if(segN1N2.getSlopeType() == SlopeType::NORMAL_SLOPE && segN2N3.getSlopeType() == SlopeType::NORMAL_SLOPE)
        {
            // setup y = mx + b for segment PQ
            float m1 = perpendicularSlope(slope(segN1N2));
            float b1 = midPoint(segN1N2).y - m1 * midPoint(segN1N2).x;

            // setup y = mx + b for segment QR
            float m2 = perpendicularSlope(slope(segN2N3));
            float b2 = midPoint(segN2N3).y - m2 * midPoint(segN2N3).x;

            // set them equal to each other
            // m1x + b1 = m2x + b2 -> m1x - m2x = b2 - b1 -> x (m1 - m2) = b2 - b1
            intersection.x = (b2 - b1) / (m1 - m2);
            intersection.y = m2 * intersection.x + b2;
            return intersection;
        }
        // right triangles
        else if(segN1N2.getSlopeType() == SlopeType::INFINITE_SLOPE && segN2N3.getSlopeType() == SlopeType::ZERO_SLOPE)
        {
            intersection.y = midPoint(segN1N2).y;
            intersection.x = midPoint(segN2N3).x;
            return intersection;
        }
        else if(segN1N2.getSlopeType() == SlopeType::ZERO_SLOPE && segN2N3.getSlopeType() == SlopeType::INFINITE_SLOPE)
        {
            intersection.y = midPoint(segN2N3).y;
            intersection.x = midPoint(segN1N2).x;
            return intersection;
        }
        // others ...
        else if(segN1N2.getSlopeType() == SlopeType::INFINITE_SLOPE && segN2N3.getSlopeType() == SlopeType::NORMAL_SLOPE)
        {
            intersection.y = midPoint(segN1N2).y;
            // for the normal line y = mx + b
            float m = perpendicularSlope(slope(segN2N3));
            // b = y - mx using midpoint of segment QR
            float b = midPoint(segN2N3).y - m * midPoint(segN2N3).x;
            // y = mx + b -> x = (y - b) / m
            intersection.x = (intersection.y - b) / m;
            return intersection;
        }
        else if(segN1N2.getSlopeType() == SlopeType::ZERO_SLOPE && segN2N3.getSlopeType() == SlopeType::NORMAL_SLOPE)
        {
            intersection.x = midPoint(segN1N2).x;
            // for the nomal line y = mx + b
            float m = perpendicularSlope(slope(segN2N3));
            // b = y - mx using midpoint of segment QR
            float b = midPoint(segN2N3).y - m * midPoint(segN2N3).x;
            // y = mx + b
            intersection.y = m * intersection.x + b;
            return intersection;
        }
        else if(segN1N2.getSlopeType() == SlopeType::NORMAL_SLOPE && segN2N3.getSlopeType() == SlopeType::INFINITE_SLOPE)
        {
            intersection.y = midPoint(segN1N2).y;
            // for the normal line y = mx + b
            float m = perpendicularSlope(slope(segN1N2));
            // b = y - mx using midpoint of segment PQ
            float b = midPoint(segN1N2).y - m * midPoint(segN1N2).x;
            // y = mx + b -> x = (y - b) / m
            intersection.x = (intersection.y - b) / m;
            return intersection;
        }
        else if(segN1N2.getSlopeType() == SlopeType::NORMAL_SLOPE && segN2N3.getSlopeType() == SlopeType::ZERO_SLOPE)
        {
            intersection.x = midPoint(segN1N2).x;
            // for the nomal line y = mx + b
            float m = perpendicularSlope(slope(segN1N2));
            // b = y - mx using midpoint of segment QR
            float b = midPoint(segN1N2).y - m * midPoint(segN1N2).x;
            // y = mx + b
            intersection.y = m * intersection.x + b;
            return intersection;
        }
        else
        {
            // invlid triangle, set the circum-center to (0, 0)
            // TODO: fix?
            return Node();
        }
    }
};

#endif // TRIANGLE_H
