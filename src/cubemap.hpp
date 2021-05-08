#pragma once

#include <glad/glad.h>
#include <string>

#include "stb_image.h"

class Cubemap {
public:
    Cubemap(const std::string& pathDirectory)
        :directory(pathDirectory), ID(0)
    {
        loadCubeTextures();
    }
    ~Cubemap(){}
    inline GLuint getID() {
        return ID;
    }
private:
    void loadCubeTextures() {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false); 
        for(unsigned int i = 0; i < faces.size(); i++) {
            std::string path = directory + "/" + faces[i];
            unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            if(data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                std::cout << "load cube map texture failure, path is: " << path << std::endl;
                stbi_image_free(data);
                return;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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