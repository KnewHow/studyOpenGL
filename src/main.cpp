#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include <vector.hpp>
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
    Shader shader(vertex_shader_path, fragment_shader_path);
    std::cout << "program: " << shader.getProgram() << std::endl;


    const math::vec4f color[] = {
        math::vec4f(0.0, 0.8, 1.0, 1.0f)
    };

    const math::vec4f positions[3] = {
        math::vec4f(0.25, -0.25, 0.5, 1.0),
        math::vec4f(-0.25, -0.25, 0.5, 1.0),
        math::vec4f(0.25, 0.25, 0.5, 1.0)
    };

    GLuint buffer[2];
    glCreateBuffers(2, &buffer[0]); // create buffer
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);

    glNamedBufferStorage(buffer[0], 1024 * 1024, NULL, GL_DYNAMIC_STORAGE_BIT); // allocate storage in memory, must use `GL_DYNAMIC_STORAGE_BIT` in here
    glNamedBufferSubData(buffer[0], 0, sizeof(positions), positions); // copy data from positions to storage allocated above
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]); // bind this to array buffer

    glBindVertexArray(VAO);
    glVertexArrayVertexBuffer(VAO, 0, buffer[0], 0, sizeof(math::vec4f));
    glVertexArrayAttribFormat(VAO, 0, 4, GL_FLOAT, GL_FALSE, 0);
    // glVertexArrayAttribBinding(VAO, 0, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glNamedBufferStorage(buffer[1], sizeof(color), color, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(VAO, 1, buffer[1], 0, sizeof(math::vec4f));
    glVertexArrayAttribFormat(VAO, 1, 4, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    

    while (!glfwWindowShouldClose(window))
    {
        const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, color);
        glUseProgram(shader.getProgram());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}