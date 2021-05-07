#include "texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& p) 
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(GL_RGB), isFlipVertically(false), wrappingMode(GL_REPEAT), filterMode(GL_LINE)
{
    loadTexture();
}

Texture::Texture(const std::string& p, bool isFlipVertically)
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(GL_RGB), isFlipVertically(isFlipVertically), wrappingMode(GL_REPEAT), filterMode(GL_LINE)
{
    loadTexture();
}

Texture::Texture(const std::string& p, GLint wrappingMode) 
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(GL_RGB), isFlipVertically(false), wrappingMode(wrappingMode), filterMode(GL_LINE)
{
    loadTexture();
}

Texture::Texture(const std::string& p, bool isFlipVertically, GLint wrappingMode) 
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p), format(GL_RGB), isFlipVertically(isFlipVertically), wrappingMode(wrappingMode), filterMode(GL_LINE)
{
    loadTexture();
}


Texture::~Texture() {
    freeData();
}

void Texture::bind() {
    if(ID != 0) {
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

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
    if(data != nullptr) {
        if(nrChannels == 1) {
            format = GL_RED;
        } else if(nrChannels == 3) {
            format = GL_RGB;
        } else if(nrChannels == 4) {
            format = GL_RGBA;
        } else {
            std::cout << "load texture failure, unsupport file format! file path is: " << path << std::endl;
            return;
        }
        glGenTextures(1, &ID);
    } else {
        std::cout << "load texture failure, path: " << path << std::endl;
    }
}

void Texture::setWrappingMode(GLint wrapping) {
    wrappingMode = wrapping;
}

void Texture::setFilterMode(GLint filter) {
    filterMode = filter;
}