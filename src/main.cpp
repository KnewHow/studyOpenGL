#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cmath>

#include "shader.hpp"

namespace
{
    void errorCallback(int error, const char *description)
    {
        fprintf(stderr, "GLFW error %d: %s\n", error, description);
    }


    time_t getCurrentTime() {
        time_t seconds;
        seconds = time(NULL);
        return seconds;
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

    std::string vertex_shader_path = "../shader/vertex.vert";
    std::string fragment_shader_path = "../shader/fragment.frag";
    Shader shader(vertex_shader_path, fragment_shader_path);
    std::cout << "program: " << shader.getProgram() << std::endl;

    GLuint vertex_array_object;
    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    
    // Set the clear color to a nice green
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    int cnt = 0;
    while (!glfwWindowShouldClose(window))
    {
        const GLfloat color[] = {1.0f, 0.0f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, color);
        glUseProgram(shader.getProgram());
        long int currentTime = getCurrentTime();
        int angle = currentTime % 360;
        float angle_rad = angle * M_PI / 180.0f;
        const GLfloat attrib[] = {
            sinf(angle_rad) * 0.5f,
            cosf(angle_rad) * 0.6f,
            0.0f,
            0.0f
        };
        GLfloat r = sinf(angle_rad) * 0.5f + 0.5f;
        GLfloat g = cosf(angle_rad) * 0.5f + 0.5f;
        const GLfloat t_color[4] = {
            r,
            g,
            0.0f,
            1.0f
        };
        std::cout << "current time: " << currentTime << ", sin(angle_rad): " << sinf(angle_rad) << ", " << "cos(angle_rad): " << cosf(angle_rad) << ", " << "red: " << r << ", green: " << g <<std::endl;
        glVertexAttrib4fv(0, attrib);
        glVertexAttrib4fv(1, t_color);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        cnt++;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}