#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

#include "stb_image.h"

class HDR {
public:
    HDR(const std::string& _path)
        :width(0), height(0), nrComponents(0), path(_path), ID(0)
    {
        loadHDR();
    }
    ~HDR(){}
    inline GLuint getID() const { return ID; }
    void bind() const {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
private:
    GLuint ID;
    int width, height, nrComponents;
    std::string path;
    
    void loadHDR() {
        stbi_set_flip_vertically_on_load(true);
        float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
        if(data) {
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        } else {
            std::cerr << "Load HDF Image error, path is: " << path << std::endl;
        }
        stbi_set_flip_vertically_on_load(false);
    }
    
};