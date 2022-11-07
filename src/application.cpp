#include "application.h"

Application::Application(GLuint screenWidth, GLuint screenHeight, const char* appTitle, GLFWframebuffersizefun fbsf)
{
    std::srand(time(0));

    std::cout << "================================================================================\n"
              << "Welcome to my Bowyer Watson Algorithm implementation with C++ and OpenGL. V.1.02\n"
              << "Press `R` to generate a new triangulation.\n"
              << "================================================================================\n";

    // Setup openGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create Window
    this->window = glfwCreateWindow(screenWidth, screenHeight, appTitle, 0, 0);

    if (!this->window)
    {
        std::cout << "Failed to create GLFWWindow\n";
        glfwTerminate();
    }
    else
        glfwMakeContextCurrent(this->window);

    // Setup callbacks
    glfwSetFramebufferSizeCallback(this->window, fbsf);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
    }
}

void Application::start(void)
{
    this->generateTriangulation();
    this->bufferData();

    while(!glfwWindowShouldClose(this->window))
    {
        this->render();
    }
    glfwTerminate();
}

void Application::generateTriangulation(void)
{

    for(int n = 0; n < this->MAX_NODES; n++)
        this->nodes.emplace_back(randomValue(), randomValue());

    // superTriangle must contain all nodes in the space ...
    Triangle superTriangle(Node(-5.0f, -2.0f), Node(0.0f, 7.0f), Node(5.0f, -2.0f));
    this->triangulation.push_back(superTriangle); // first triangle must be superTriangle
    // other vars
    std::vector<Triangle> badTriangles = {};
    std::vector<Segment> polygon = {};
    /// Begin Bowyer Watson Algorithm -----------------------------------------
    // for each point in pointList do
    for(auto node = this->nodes.begin(); node != this->nodes.end(); node++)
    {
        // badTriangles := empty set
        badTriangles.clear();
        // for each triangle in triangulation do
        for(auto tri = this->triangulation.begin(); tri != this->triangulation.end(); tri++)
        {
            // if point is inside circumcircle of triangle
            tri->setIsBad(tri->isNodeInCircumCircle(*node));

            if(tri->getIsBad())
                badTriangles.push_back(*tri);
        }

        // polygon := empty set
        polygon.clear();
        // for each triangle in badTriangles do
        for(auto tri = badTriangles.begin(); tri != badTriangles.end(); tri++)
        {
            // for each edge in triangle do
            for(int i = 0; i < 3; i++)
            {
                bool isSharedSeg = false;
                //  if edge is not shared by any other triangles in badTriangles
                for(auto tri2 = badTriangles.begin(); tri2 != badTriangles.end(); tri2++)
                {
                    // if triangles are the same
                    // skip this loop -> to next triangle
                    if(tri == tri2) continue;

                    for(int j = 0; j < 3; j++)
                    {
                        if( tri->segArray.at(i) == tri2->segArray.at(j) )
                        {
                            isSharedSeg = true;
                        }
                    }
                }
                // add edge to polygon
                if(!isSharedSeg)
                    polygon.push_back(tri->segArray.at(i));
            }
        }

        for(auto tri = this->triangulation.begin(); tri != this->triangulation.end(); tri++)
        {
            // if triangle contains a vertex from original super-triangle
            if(tri->getIsBad())
                this->triangulation.erase(tri--);
        }

        // for each edge in polygon do
        for(auto seg = polygon.begin(); seg != polygon.end(); seg++)
        {
            // newTri := form a triangle from edge to point
            // add newTri to triangulation
            this->triangulation.emplace_back(*seg, *node);
        }
    }
    /*
    // for each triangle in triangulation
    for(auto tri = this->triangulation.begin(); tri != this->triangulation.end(); tri++)
    {
        // if triangle contains a vertex from original super-triangle
        if(tri->containsNodesOf(superTriangle) )
            this->triangulation.erase(tri--);
    }*/
}

void Application::bufferData(void)
{
    this->shader = Shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    this->shader.use();

    glGenVertexArrays(1, &this->vertexArray);
    glGenBuffers(1, &this->vertexBuffer);

    std::vector<Vertex> data;
    this->formatData(&data);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    if(data.size() > 0)
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), &data.at(0), GL_STATIC_DRAW);
    else
        std::cout << "Error, triangles were not generated, no data to render.\n";

    glBindVertexArray(this->vertexArray);
    // Position attributes of triangulation data
    // Takes up 3 floats: x, y, and z.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attributes of triangulation data
    // Takes up 3 floats: r, g, and b.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* uncomment this to see wire frames of triangles */
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // set frame rate to 60
    glfwSwapInterval(1);
}

void Application::formatData(std::vector<Vertex>* data) const
{
    std::array<float, 3>  color;
    const float Z = 0.0f;

    for(std::vector<Triangle>::const_iterator tri = this->triangulation.cbegin(); tri != this->triangulation.cend(); tri++)
    {
        color = {std::abs(randomValue()), std::abs(randomValue()), std::abs(randomValue())};
        data->emplace_back(tri->nodesArray.at(0).x, tri->nodesArray.at(0).y, Z, color.at(0), color.at(1), color.at(2));
        data->emplace_back(tri->nodesArray.at(1).x, tri->nodesArray.at(1).y, Z, color.at(0), color.at(1), color.at(2));
        data->emplace_back(tri->nodesArray.at(2).x, tri->nodesArray.at(2).y, Z, color.at(0), color.at(1), color.at(2));
    }
}

void Application::render(void)
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (glfwGetKey(this->window, GLFW_KEY_R))
    {
        this->triangulation.clear();
        this->nodes.clear();
        this->generateTriangulation();
        this->bufferData();
    }

    glDrawArrays(GL_TRIANGLES, 0, this->triangulation.size() * 3);

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}
