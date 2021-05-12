#pragma once

#include "glad/glad.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <optional>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.hpp"
#include "mesh.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace Model
{
    class Model {
    public:
        Model(const std::string& path)
            :meshs(), textureCache(), direcoty("")
        {
            loadModel(path);
        }

        void draw(const Shader& shader) {
            for(const auto& mesh: meshs)
                mesh.draw(shader);
        }

        std::vector<Mesh> getMesh() const {
            return meshs;
        }

        std::optional<Texture> getCachedTextureByName(const std::string& name) const {
            auto res = textureCache.find(name);
            if(res != textureCache.end()) {
                return res->second;
            }
            return std::nullopt;
        }

    private:
        void loadModel(const std::string& path) {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cout << "Load model failure, path is: " << path << ", errMsg is: " << importer.GetErrorString() << std::endl;
                return;
            }
            direcoty = path.substr(0, path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }

        void processNode(const aiNode* node, const aiScene* scene) {
            for(unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshs.push_back(processMesh(mesh, scene));
            }

            for(unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }

        }

        Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            // deal vertex
            for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;
                glm::vec3 vector;

                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;

                if(mesh->HasNormals()) {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }

                if(mesh->mTextureCoords[0]) {
                    glm::vec2 vec; // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.texCoords = vec;

                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.tangent = vector;

                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.bitangent = vector;

                }
                vertices.push_back(vertex);
            }

            // deal index
            for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            // deal texture
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            std::vector<Texture> diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"); // diffuse texture
            textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

            std::vector<Texture> specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"); // specular texture
            textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

            std::vector<Texture> normalTextures = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal"); // normal texture
            textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());

            std::vector<Texture> heightTextures = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height"); // height texture
            textures.insert(textures.end(), heightTextures.begin(), heightTextures.end());

            return Mesh(vertices, indices, textures);
        }

        std::vector<Texture> loadMaterialTextures(const aiMaterial* material, aiTextureType type, const std::string& typeName) {
            std::vector<Texture> res;

            for(unsigned int i = 0; i < material->GetTextureCount(type); i++) {
                aiString str;
                material->GetTexture(type, i, &str);
                std::string filename = std::string(str.C_Str());
                if(textureCache.find(filename) != textureCache.end()) {
                    res.push_back(textureCache.find(filename)->second);
                } else {
                    auto idOpt = textureFromFile(filename, this->direcoty);
                    if(idOpt.has_value()) {
                        Texture texture;
                        texture.id = idOpt.value();
                        texture.type = typeName;
                        res.push_back(texture);
                        textureCache.insert(std::make_pair(filename, texture));
                    }
                }
            }

            return res;
        }

        std::optional<unsigned int> textureFromFile(const std::string& filename, const std::string& dir, bool gamma = false) {
            std::string filepath = dir + "/" + filename;

            int width, height, nrComponents;
            stbi_set_flip_vertically_on_load(true); 
            unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
            if(data) {
                GLenum format;
                if(nrComponents == 1) {
                    format = GL_RED;
                } else if(nrComponents == 3) {
                    format = GL_RGB;
                } else if(nrComponents == 4) {
                    format = GL_RGBA;
                } else {
                    std::cerr << "load texture failure, unsupport this format: " << nrComponents  << ", file path is: " << filepath << std::endl;
                    stbi_image_free(data);
                    return std::nullopt;
                } 
                unsigned int id;
                glGenTextures(1, &id);

                glBindTexture(GL_TEXTURE_2D, id);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                
                stbi_image_free(data);
                return id;
            } else {
                std::cerr << "load texture failure, error path: " << filepath << std::endl;
                stbi_image_free(data);
                return std::nullopt;
            }
        }

        std::vector<Mesh> meshs;
        std::unordered_map<std::string, Texture> textureCache;
        std::string direcoty; // model located directory, use it to load texture
    };
}
