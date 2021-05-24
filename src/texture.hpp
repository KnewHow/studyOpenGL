#pragma once

#include <string>
#include <glad/glad.h>

class Texture {

private:
    GLuint ID;  
    
    int width, height, nrChannels;
    std::string path;
    unsigned char* data;
   

    bool isFlipVertically;
    bool withGammaCorrection;

    GLint wrappingMode;
    GLint filterMode;
    GLenum format;
    GLenum internalFormat;

    void loadTexture();
public:
    Texture(const std::string& p, bool _withGammaCorrection = false);
    Texture(const std::string& p, bool isFlipVertically, bool _withGammaCorrection = false);
    Texture(const std::string& p, GLint wrappingMode, bool _withGammaCorrection = false);
    Texture(const std::string& p, bool isFlipVertically, GLint wrappingMode, bool _withGammaCorrection = false);
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