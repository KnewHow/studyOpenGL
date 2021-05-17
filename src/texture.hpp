#pragma once

#include <string>
#include <glad/glad.h>

class Texture {

private:
    int width, height, nrChannels;
    std::string path;
    GLuint ID;
    unsigned char* data;
    GLenum format;
    bool isFlipVertically;
    GLint wrappingMode;
    GLint filterMode;
    void loadTexture();
public:
    Texture(const std::string& p);
    Texture(const std::string& p, bool isFlipVertically);
    Texture(const std::string& p, GLint wrappingMode);
    Texture(const std::string& p, bool isFlipVertically, GLint wrappingMode);
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
    void setWrappingMode(GLint wraping);
    void setFilterMode(GLint filter);
};