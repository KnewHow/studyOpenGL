#pragma once

#include <string>
#include <optional>
#include <vector>

#include "glad/glad.h"
#include "matrix.hpp"
#include "vec3.hpp"

class Shader {
public:
    enum class ShaderType {
        Vertex = 1,
        Fragment = 2,
        Geometry = 3
    };
    Shader(const std::string& v_path, const std::string& f_path);
    Shader(const std::string& v_path, const std::string& f_path, const std::string& g_path);
    ~Shader();
    inline GLuint getProgram() const { return program; }
    inline void use() const { glUseProgram(program); }
    void setVec3f(const std::string& name, const glm::vec3& v) const;
    void setVec3f(const std::string& name, float x, float y, float z) const;
    /**
     * set uniform vec4f 
     * @param name the name of uniform variable
     * @param {x, y, z, w} the vec4f
    */
    void setVec4f(const std::string& name, float x, float y, float z, float w) const;
    void setBool(const std::string& name, bool v) const; // set bool uniform variable 
    void setInt(const std::string name, int v) const; // set int uniform variable
    void setFloat(const std::string& name, float v) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setMat4s(const std::string& name, const std::vector<glm::mat4> mats) const;
    inline void destory() const { glDeleteProgram(program); } 

private:
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    std::optional<std::string> geometry_shader_path;
    GLuint program;
    std::string loadFile(const std::string& filepath);
    GLuint compileShader(const std::string& shaderContent, const ShaderType& type);
    GLuint createProgram();
   
};