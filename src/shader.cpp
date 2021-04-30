#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "gtc/type_ptr.hpp"
#include "debugGL.hpp"



Shader::Shader(const std::string& v_path, const std::string& f_path)
    :vertex_shader_path(v_path), fragment_shader_path(f_path)
{
    program = createProgram();
}

Shader::~Shader() {
    std::cout << "Shader destroy!" << std::endl;
    //glDeleteProgram(program);
}

std::string Shader::loadFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if(!in.is_open()) {
        std::cerr << "Error shader path: " << filepath << std::endl;
        return "";
    }
    std::ostringstream ss = std::ostringstream();
    ss << in.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(const std::string& shaderContent, const ShaderType& type) {
    GLuint shader = glCreateShader(type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
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
    std::string vertex_shader_content = loadFile(vertex_shader_path);
    std::string fragment_shader_content = loadFile(fragment_shader_path);
    GLuint vertex_shader = compileShader(vertex_shader_content, ShaderType::Vertex);
    GLuint fragment_shader = compileShader(fragment_shader_content, ShaderType::Fragment);
    GLuint program = 0;
    if(vertex_shader != GL_FALSE && fragment_shader != GL_FALSE) {
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        GLint success = false;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success) {
            GLint length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            std::vector<GLchar> log(length);
            glGetProgramInfoLog(program, length, &length, &log[0]);
            std::string err_msg = std::string(log.begin(), log.end());
            std::cout << "shader link error: " << err_msg << std::endl;
        }
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}

void Shader::setVec3f(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec4f(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}


void Shader::setBool(const std::string& name, bool v) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)v);
}

void Shader::setInt(const std::string name, int v) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), v);
}

void Shader::setFloat(const std::string& name, float v) const {
     glUniform1f(glGetUniformLocation(program, name.c_str()), v);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}