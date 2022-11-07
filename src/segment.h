#ifndef SEGMENT_H
#define SEGMENT_H

#include "node.h"

enum SlopeType
{
    NORMAL_SLOPE,
    ZERO_SLOPE,
    INFINITE_SLOPE
};

class Segment
{
public:
    Segment() = default;
    Segment(Node p_, Node q_) : p(p_), q(q_), slopeType(SlopeType::NORMAL_SLOPE) {}

    inline friend bool operator == (const Segment& s1, const Segment& s2) noexcept
    {
        return (s1.p == s2.p && s1.q == s2.q) || (s1.p == s2.q && s1.q == s2.p);
    }

    inline friend bool operator != (const Segment& s1, const Segment& s2) noexcept
    {
        return (s1.p != s2.p) || (s1.q != s2.q);
    }

    inline void setSlopeType(const SlopeType& value) noexcept
    {
        this->slopeType = value;
    }

    inline SlopeType getSlopeType(void) const noexcept
    {
        return this->slopeType;
    }

public:
    Node p, q;

private:
    SlopeType slopeType;
};

#endif // SEGMENT_H
