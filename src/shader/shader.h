//
// Created by Tyler Sheehan on 4/8/25.
//

#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using std::string, std::ifstream, std::stringstream, std::cout, std::endl;


class Shader {
public:
    unsigned int ID;

    Shader() {}

    Shader &use();

    void compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);

    void setFloat(const char *name, float value) const;

    void setInteger(const char *name, int value) const;

    void setVector2f(const char *name, float x, float y) const;

    void setVector2f(const char *name, const glm::vec2 &value) const;

    void setVector3f(const char *name, float x, float y, float z) const;

    void setVector3f(const char *name, const glm::vec3 &value) const;

    void setVector4f(const char *name, float x, float y, float z, float w) const;

    void setVector4f(const char *name, const glm::vec4 &value) const;

    void setMatrix4(const char *name, const glm::mat4 &matrix) const;

private:
    void checkCompileErrors(unsigned int object, std::string type);

};



#endif //SHADER_H
