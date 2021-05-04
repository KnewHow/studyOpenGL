#pragma once

#include "glad/glad.h"

#include <string>
#include <vector>

#include "vec3.hpp"
#include "shader.hpp"

namespace Model {
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
    };

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vs, const std::vector<unsigned int>& is, const std::vector<Texture>& ts)
            :vertices(vs), indices(is), textures(ts), VAO(0), VBO(0), EBO(0)
        {
            setupMesh();
        }

        void draw(const Shader& shader) const {
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            for(unsigned int i = 0; i < textures.size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string number;
                std::string type = textures[i].type;
                if(type == "texture_diffuse") {
                    number = std::to_string(diffuseNr++);
                } else if(type == "texture_specular") {
                    number = std::to_string(specularNr++);
                }
                shader.setFloat("material." + type + number, i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
    private:
        void setupMesh() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);    
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));    
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));    
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
        }


        GLuint VAO, VBO, EBO;
        
    };
}


