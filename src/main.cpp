#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "vector.hpp"
#include "matrix.hpp"
#include "shader.hpp"
#include "model/model.hpp"
#include "debugGL.hpp"

constexpr int width = 1366;
constexpr int height = 768;


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

    GLFWwindow *window = glfwCreateWindow(width, height, "StudyOpenGL", nullptr, nullptr);
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
    std::string model_path = "../res/cube/cube.obj";
    std::vector<int> v;
    Model m(model_path);

    const math::vec4f color[] = {
        math::vec4f(0.0, 0.8, 1.0, 1.0f)
    };

    // static const GLfloat positions[] =
    // {
    //     0.25f,  0.25f, -0.25f,
    //     -0.25f, -0.25f, -0.25f,
    //     -0.25f, 0.25f, -0.25f,
    // };

    static const GLfloat positions[] =
    {
      -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
    };

    // std::vector<math::vec4f> positions = m.getTriangles();
    // std::cout << "triangle size: " << positions.size() << std::endl;

    std::cout << "sizeof(float): " << sizeof(float) << ", sizeof(vec4f): " << sizeof(math::vec4f) << std::endl; 

    GLuint mv_mat_id = glGetUniformLocation(shader.getProgram(), "model_view_mat");
    GLuint proj_mat_id = glGetUniformLocation(shader.getProgram(), "projection_mat");
    
  
    GLuint VAO;
    glCall(glCreateVertexArrays(1, &VAO));
    glCall(glBindVertexArray(VAO));

    // GLuint buffer[2];
    // glCreateBuffers(2, &buffer[0]); // create buffer
    // glNamedBufferStorage(buffer[0], 1024 * 1024, NULL, GL_DYNAMIC_STORAGE_BIT); // allocate storage in memory, must use `GL_DYNAMIC_STORAGE_BIT` in here
    // glNamedBufferSubData(buffer[0], 0, sizeof(positions), positions); // copy data from positions to storage allocated above
    // glBindBuffer(GL_ARRAY_BUFFER, buffer[0]); // bind this to array buffer

    // glVertexArrayVertexBuffer(VAO, 0, buffer[0], 0, sizeof(math::vec4f));
    // glVertexArrayAttribFormat(VAO, 0, 4, GL_FLOAT, GL_FALSE, 0);
    // glEnableVertexArrayAttrib(VAO, 0);

    // glNamedBufferStorage(buffer[1], sizeof(color), color, GL_DYNAMIC_STORAGE_BIT);
    // glVertexArrayVertexBuffer(VAO, 1, buffer[1], 0, sizeof(math::vec4f));
    // glVertexArrayAttribFormat(VAO, 1, 4, GL_FLOAT, GL_FALSE, 0);
    // glEnableVertexArrayAttrib(VAO, 1);

    math::mat4f mv = math::rotateX(30.0f);
    math::mat4f perspective = math::perspective(50.0f, (float)width/height, 0.1f, 1000.0f);

    GLuint buffer;
    glCall(glGenBuffers(1, &buffer));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    glCall(glBufferData(GL_ARRAY_BUFFER,
                    sizeof(positions),
                    positions,
                    GL_STATIC_DRAW));
    glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL));
    glCall(glEnableVertexAttribArray(0));

    glCall(glEnable(GL_CULL_FACE));
    glCall(glFrontFace(GL_CCW));
    glCall(glEnable(GL_DEPTH_TEST));
    glCall(glDepthFunc(GL_LEQUAL));

    while (!glfwWindowShouldClose(window))
    {
        const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
        const GLfloat one = 1.0f;

        glCall(glViewport(0, 0, width, height));
        glCall(glClearBufferfv(GL_COLOR, 0, color));
        glCall(glClearBufferfv(GL_DEPTH, 0, &one));
        glCall(glUseProgram(shader.getProgram()));
        glUniformMatrix4fv(mv_mat_id, 1, GL_FALSE, &mv[0][0]);
        glUniformMatrix4fv(proj_mat_id, 1, GL_FALSE, &perspective[0][0]);
        glCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        
        glCall(glfwSwapBuffers(window));
        glCall(glfwPollEvents());
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}