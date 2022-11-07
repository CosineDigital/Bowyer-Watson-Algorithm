#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "C:\C++ Libraries\glad\include\glad\glad.h"


class Shader
{
public:
    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);
    unsigned int getId(void);
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(GLuint, std::string);
    unsigned int Id;
};

