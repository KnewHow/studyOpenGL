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

private:
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    GLuint program;
    std::string loadFile(const std::string& filepath);
    GLuint compileShader(const std::string& shaderContent, const ShaderType& type);
    GLuint createProgram();
   
};