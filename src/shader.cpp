#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(
    const std::string& v_path, 
    const std::string& f_path,
    const std::string& tess_c_path,
    const std::string& tess_e_path, 
    const std::string& g_path)
        :vertex_shader_path(v_path), 
        fragment_shader_path(f_path), 
        tess_control_shader_path(tess_c_path), 
        tess_evaluation_shader_path(tess_e_path),
        geometry_shader_path(g_path)
{
    program = createProgram();
}

Shader::~Shader() {
    glDeleteProgram(program);
}

std::optional<std::string> Shader::loadFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if(!in.is_open()) {
        std::cerr << "Warning: shader path " << filepath << std::endl;
        return std::nullopt;
    }
    std::ostringstream ss = std::ostringstream();
    ss << in.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(const std::string& shaderContent, const GLenum& shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* c_str = shaderContent.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    GLint isCompiled = 0; // error check
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorInfo(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorInfo[0]);
        std::string errMsg(errorInfo.begin(), errorInfo.end());
        std::cerr << "Compile shader error, shader file content is: " << std::endl << 
                      shaderContent << std::endl << "error is: " << errMsg << std::endl;

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Shader::createProgram() {
    auto vertex_shader_content = loadFile(vertex_shader_path);
    auto fragment_shader_content = loadFile(fragment_shader_path);
    auto tess_control_shader_content = loadFile(tess_control_shader_path);
    auto tess_evaluation_shader_content = loadFile(tess_evaluation_shader_path);
    auto geometry_shader_content = loadFile(geometry_shader_path);
    GLuint p = glCreateProgram();

    GLuint vertex_shader = 0;
    if(vertex_shader_content.has_value()) {
        GLuint vertex_shader = compileShader(vertex_shader_content.value(), GL_VERTEX_SHADER);
        glAttachShader(p, vertex_shader);
    }

    GLuint fragment_shader = 0;
    if(fragment_shader_content.has_value()) {
        fragment_shader = compileShader(fragment_shader_content.value(), GL_FRAGMENT_SHADER);
        glAttachShader(p, fragment_shader);
    }


    GLuint tess_control_shader = 0;
    if(tess_control_shader_content.has_value()) {
        tess_control_shader = compileShader(tess_control_shader_content.value(), GL_TESS_CONTROL_SHADER);
        glAttachShader(p, tess_control_shader);
    }

    GLuint tess_evaluation_shader = 0;
    if(tess_evaluation_shader_content.has_value()) {
        tess_evaluation_shader = compileShader(tess_evaluation_shader_content.value(), GL_TESS_EVALUATION_SHADER);
        glAttachShader(p, tess_evaluation_shader);
    }

    GLuint geometry_shader = 0;
    if(geometry_shader_content.has_value()) {
        geometry_shader = compileShader(geometry_shader_content.value(), GL_GEOMETRY_SHADER);
        glAttachShader(p, geometry_shader);
    }

    glLinkProgram(p);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(tess_control_shader);
    glDeleteShader(tess_evaluation_shader);
    glDeleteShader(geometry_shader);
    return p;
}