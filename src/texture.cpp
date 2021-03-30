#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"




Texture::Texture(const std::string& p) 
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(GL_RGB), isFlipVertically(false), wrappingMode(GL_REPEAT)
{
    loadTexture();
}

Texture::Texture(const std::string& p, GLenum format)
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(format), isFlipVertically(false), wrappingMode(GL_REPEAT)
{
    loadTexture();
}


Texture::Texture(const std::string& p,  GLenum format, bool isFlipVertically)
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(format), isFlipVertically(isFlipVertically), wrappingMode(GL_REPEAT)
{
    loadTexture();
}

Texture::Texture(const std::string& p,  GLenum format, bool isFlipVertically, GLint wrappingMode) 
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(format), isFlipVertically(isFlipVertically), wrappingMode(wrappingMode)
{
    loadTexture();
}


Texture::~Texture() {
    freeData();
}

void Texture::bind() {
    if(data != nullptr) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}


void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::freeData() {
    if(data != nullptr) {
        stbi_image_free(data);
        data = nullptr;
    }
}

void Texture::loadTexture() {
    stbi_set_flip_vertically_on_load(isFlipVertically); 
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
}

void Texture::setWrappingMode(GLint mode) {
    wrappingMode = mode;
}
