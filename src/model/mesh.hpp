#pragma once

#include "glad/glad.h"

#include <string>
#include <vector>

#include "vec3.hpp"
#include "shader.hpp"

namespace Model {
    struct Vertex
    {
        Vertex() {
            position = glm::vec3(0);
            normal = glm::vec3(0);
            texCoords = glm::vec3(0);
            tangent = glm::vec3(0);
            bitangent = glm::vec3(0);
        }
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    struct Texture {
        Texture() {
            id = 0;
            type = "";
        }
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

        void bindTexture(const Shader& shader) const {
             unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            for(unsigned int i = 0; i < textures.size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string number;
                std::string type = textures[i].type;
                if(type == "texture_diffuse") {
                    number = std::to_string(diffuseNr++);
                } else if(type == "texture_specular") {
                    number = std::to_string(specularNr++);
                } else if(type == "texture_normal") {
                    number = std::to_string(normalNr++);
                } else if(type == "texture_height") {
                     number = std::to_string(heightNr++);
                }
                shader.setFloat("material." + type + number, i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glActiveTexture(GL_TEXTURE0);
        }

        void draw(const Shader& shader) const {
           
            bindTexture(shader);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        inline GLuint getVAO() const {
            return VAO;
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


