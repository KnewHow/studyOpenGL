#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(const std::string& p)
    :width(0), height(0), nrChannels(0), ID(0), data(nullptr), path(p)
{
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
}

Texture::~Texture() {
    freeData();
}

void Texture::bind() {
    if(data != nullptr) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
