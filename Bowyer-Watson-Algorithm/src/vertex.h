#ifndef VERTEX_H
#define VERTEX_H

/** a substitute to using an array of 6 floats*/
class Vertex
{
public:
    Vertex() = default;
    Vertex(float x_, float y_, float z_, float r_, float g_, float b_)
            : x(x_), y(y_), z(z_), r(r_), g(g_), b(b_) {}
public:
    float x, y, z, r, g, b;
};

#endif // VERTEX_H
