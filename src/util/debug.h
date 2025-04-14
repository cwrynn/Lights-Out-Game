//
// Created by Tyler Sheehan on 4/8/25.
//

#ifndef DEBUG_H
#define DEBUG_H

#include <glad/glad.h>
#include <iostream>

GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
            break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
            break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
            break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
            break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
            break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
            break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define glFunction(func, ...) func(__VA_ARGS__); CHECK_GL_ERROR()

#endif //DEBUG_H
