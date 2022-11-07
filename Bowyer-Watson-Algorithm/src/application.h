#ifndef APPLICATION_H
#define APPLICATION_H

#include <ctime>
#include <array>
#include <vector>
#include <random>
#include <iostream>
#include "C:\C++ Libraries\glad\include\glad\glad.h"

#include "node.h"
#include "vertex.h"
#include "shader.h"
#include "segment.h"
#include "segment.h"
#include "triangle.h"
#include "callbacks.h"
#include "utilities.h"

class Application
{
public:
    explicit Application(GLuint screenWidth, GLuint screenHeight, const char* appTitle, GLFWframebuffersizefun fbsf);

    void start(void);

private:
    void generateTriangulation(void);
    void formatData(std::vector<Vertex>* data) const;
    void bufferData(void);
    void render(void);

    GLFWwindow* window;
    Shader shader;
    unsigned int vertexBuffer;
    unsigned int vertexArray;

    const int MAX_NODES = 99;
    std::vector<Triangle> triangulation;
    std::vector<Node> nodes;
};

#endif // APPLICATION_H
