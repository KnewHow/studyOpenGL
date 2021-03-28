#pragma once

#include <glad/glad.h>
#include <iostream>


static void glClearError() {
    while(glGetError() != GL_NO_ERROR);
}

static bool glLogCall(const char* functionName, const char* filename, int line) {
    while(GLenum error = glGetError()) {
        std::cout << "OpenGL error (" << error << "), " << filename << "， " << functionName << ": " << line << std::endl;
        return false;
    }
    return true;
}

// #define ASSERT(x) if(!(x)) return;

#define glCall(x) glClearError();\
    x;\
    glLogCall(#x, __FILE__, __LINE__)

