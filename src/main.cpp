#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/string_cast.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "model/mesh.hpp"
#include "model/model.hpp"
//#include "debugGL.hpp"

const int width = 1366;
const int height = 768;

float mixValue = 0.2;

Camera _camera(glm::vec3(0.0f, 0.0f, 3.0f));


bool firstMouse = true;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

namespace
{
    void errorCallback(int error, const char *description)
    {
        fprintf(stderr, "GLFW error %d: %s\n", error, description);
    }

    void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
       _camera.processZoom(yoffset);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if(firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;
        _camera.processLookAround(xOffset, yOffset, true);
    }

    /**
     * resize viewport when window resize
    */
    void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
        glViewport(0, 0, w, h);
    }

    /**
     * deal with input from user
    */
    void processInput(GLFWwindow* window) {
        const float cameraSpeed = 2.5 * deltaTime;
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        } else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            mixValue -= 0.001;
            if(mixValue < 0.0f) {
                mixValue = 0.0f;
            }
        } else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            mixValue += 0.001;
            if(mixValue > 1.0f) {
                mixValue = 1.0f;
            }
        } else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            _camera.processMovement(camera::Movement::FORWARD, deltaTime);
        } else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
           _camera.processMovement(camera::Movement::BACKWARD, deltaTime);
        } else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            _camera.processMovement(camera::Movement::LEFT, deltaTime);
        } else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            _camera.processMovement(camera::Movement::RIGHT, deltaTime);
        }
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
}

int main(int argc, char *argv[])
{
    glfwSetErrorCallback(errorCallback);

    GLFWwindow *window = initialize();
    if (!window)
    {
        return 0;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    
    Shader greenCubeShader("../shader/cubes/vertex.glsl", "../shader/cubes/greenFragment.glsl");
    Shader redCubeShader ("../shader/cubes/vertex.glsl", "../shader/cubes/redFragment.glsl");

    GLfloat cubeVerties[] = {
        // positions         
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f
    };

    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerties), &cubeVerties, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint uniformBlockRedIndex = glGetUniformBlockIndex(greenCubeShader.getProgram(), "Matrices");
    GLuint uniformBlockGreenIndex = glGetUniformBlockIndex(redCubeShader.getProgram(), "Matrices");

    glUniformBlockBinding(redCubeShader.getProgram(), uniformBlockRedIndex, 0);
    glUniformBlockBinding(greenCubeShader.getProgram(), uniformBlockGreenIndex, 0);

    GLuint uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));



    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // calculate frame interval
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view;
        view = _camera.getViewMat();
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(_camera.getZoom()), (float)width/height, 0.1f, 100.0f);
        
       
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindVertexArray(cubeVAO);
        redCubeShader.use();
        model = glm::translate(model, glm::vec3(-0.75, 0.75, 0));
        redCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        greenCubeShader.use();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.75, 0.75, 0.0));
        greenCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "fps:" << (int)(1 / deltaTime) << std::endl;
    return 0;
}