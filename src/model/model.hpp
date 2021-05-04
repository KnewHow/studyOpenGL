#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.hpp"
#include "mesh.hpp"


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
            // TODO





            return Mesh(vertices, indices, textures);
        }

        std::vector<Texture> loadMaterialTextures(const aiMaterial* material, aiTextureType type, const std::string& name) {
            // TODO
        }

        std::vector<Mesh> meshs;
        std::unordered_map<std::string, Texture> textureCache;
        std::string direcoty; // model located directory, use it to load texture
    };
}
