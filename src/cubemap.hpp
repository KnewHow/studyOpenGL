#pragma once

#include <glad/glad.h>
#include <string>

class Cubemap {
    Cubemap(const std::string& pathDirectory)
        :directory(pathDirectory), ID(0)
    {
        loadCubeTextures();
    }
    ~Cubemap(){}
private:
    void loadCubeTextures() {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        // TODO
    }
    GLuint ID;
    std::string directory;
    const std::vector<std::string> faces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
};