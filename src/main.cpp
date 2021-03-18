#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "shader.hpp"

namespace
{
    void errorCallback(int error, const char *description)
    {
        fprintf(stderr, "GLFW error %d: %s\n", error, description);
    }

    GLFWwindow *initialize()
    {
        int glfwInitRes = glfwInit();
        if (!glfwInitRes)
        {
            fprintf(stderr, "Unable to initialize GLFW\n");
            return nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow *window = glfwCreateWindow(1366, 768, "StudyOpenGL", nullptr, nullptr);
        if (!window)
        {
            fprintf(stderr, "Unable to create GLFW window\n");
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window);

        int gladInitRes = gladLoadGL();
        if (!gladInitRes)
        {
            fprintf(stderr, "Unable to initialize glad\n");
            glfwDestroyWindow(window);
            glfwTerminate();
            return nullptr;
        }

        return window;
    }
}

int main(int argc, char *argv[])
{
    glfwSetErrorCallback(errorCallback);

    GLFWwindow *window = initialize();
    if (!window)
    {
        return 0;
    }

    std::string vertex_shader_path = "../shader/vertex.glsl";
    std::string fragment_shader_path = "../shader/fragment.glsl";
    std::string tess_control_path = "../shader/tess_control.glsl";
    std::string tess_evaluation = "../shader/tess_evaluation.glsl";
    Shader shader(vertex_shader_path, fragment_shader_path, tess_control_path, tess_evaluation);
    std::cout << "program: " << shader.getProgram() << std::endl;

    GLuint vertex_array_object;
    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window))
    {
        const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, color);
        glUseProgram(shader.getProgram());
        glDrawArrays(GL_PATCHES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}