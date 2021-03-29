#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
public:
    enum class ShaderType {
        Vertex = 1,
        Fragment = 2
    };
    Shader(const std::string& v_path, const std::string& f_path);
    ~Shader();
    inline GLuint getProgram() const { return program; }
    inline void use() const { glUseProgram(program); }
    /**
     * set uniform vec4f 
     * @param name the name of uniform variable
     * @param {x, y, z, w} the vec4f
    */
    void setVec4f(const std::string& name, float x, float y, float z, float w) const;
    void setBool(const std::string& name, bool v) const; // set bool uniform variable 
    void setInt(const std::string name, int v) const; // set int uniform variable
    void setFloat(const std::string& name, float v) const;
    
    inline void destory() const { glDeleteProgram(program); } 

private:
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    GLuint program;
    std::string loadFile(const std::string& filepath);
    GLuint compileShader(const std::string& shaderContent, const ShaderType& type);
    GLuint createProgram();
   
};