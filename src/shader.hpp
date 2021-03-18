#pragma once

#include <string>
#include <glad/glad.h>
#include <optional>

class Shader {
public:
    Shader(
        const std::string& v_path, 
        const std::string& f_path,
        const std::string& tess_c_path,
        const std::string& tess_e_path);
    ~Shader();
    inline GLuint getProgram() const { return program; }

private:
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    std::string tess_control_path;
    std::string tess_evaluation_path;
    GLuint program;
    std::optional<std::string> loadFile(const std::string& filepath);
    GLuint compileShader(const std::string& shaderContent, const GLenum& shaderType);
    GLuint createProgram();
};