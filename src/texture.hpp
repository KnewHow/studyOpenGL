#pragma once

#include <string>
#include <glad/glad.h>

class Texture {

private:
    int width, height, nrChannels;
    std::string path;
    GLuint ID;
    unsigned char* data;
public:
    Texture(const std::string& p);
    ~Texture();
    inline int getWidth() { return width; }
    inline int getHeight() { return height; }
    inline GLuint getID() { return ID; }
    /**
     * bind current texture
    */
    void bind();
    void unbind();
    void freeData();
};