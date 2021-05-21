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


GLuint planeVAO = 0;
GLuint planeVBO = 0;

void renderPlane(const Shader& shader) {
    if(planeVAO == 0) {
        glm::vec3 pos1(-1.0, 1.0, 0.0f);
        glm::vec3 pos2(-1.0, -1.0, 0.0f);
        glm::vec3 pos3(1.0, -1.0, 0.0f);
        glm::vec3 pos4(1.0, 1.0, 0.0f);

        glm::vec2 uv1(0.0, 1.0);
        glm::vec2 uv2(0.0, 0.0);
        glm::vec2 uv3(1.0, 0.0);
        glm::vec2 uv4(1.0, 1.0);
        
        glm::vec3 nm(0.0, 0.0, 1.0);

        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;

        // deal triangle 1

        glm::vec3 e1 = pos2 - pos1;
        glm::vec3 e2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        tangent1.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);  
        tangent1.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
        tangent1.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);

        // bitangent1.x = f * (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
        // bitangent1.y = f * (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
        // bitangent1.z = f * (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);

        // deal triangle 2
        e1 = pos3 - pos1;
        e2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);  
        tangent2.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
        tangent2.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);

        // bitangent2.x = f * (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
        // bitangent2.y = f * (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
        // bitangent2.z = f * (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);

        float planeData[] = {
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z
        };

        glGenVertexArrays(1, &planeVAO);
        glBindVertexArray(planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeData), &planeData, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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

    Shader objShader("../shader/object/vertex.glsl", "../shader/object/fragment.glsl");
    Texture diffuseTexture("../res/texture/wood1.png");
    Texture normalTexture("../res/texture/toy_box_normal.png");
    Texture displacementTexture("../res/texture/toy_box_disp.png");

    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
    float height_scale = 0.1;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view;
        view = _camera.getViewMat();
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(_camera.getZoom()), (float)width/height, 0.1f, 100.0f);
        
        objShader.use();
        objShader.setMat4("view", view);
        objShader.setMat4("projection", projection);
        // model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0,1.0)));
        objShader.setMat4("model", model);
        objShader.setVec3f("lightPos", lightPos);
        objShader.setVec3f("viewerPos", _camera.getPosition());
        objShader.setFloat("height_scala", height_scale);
        objShader.setInt("diffuse_texture", 0);
        glActiveTexture(GL_TEXTURE0);
        diffuseTexture.bind();
        objShader.setInt("normal_texture", 1);
        glActiveTexture(GL_TEXTURE1);
        normalTexture.bind();
        objShader.setInt("displacement_texture", 2);
        glActiveTexture(GL_TEXTURE2);
        displacementTexture.bind();
        renderPlane(objShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        // std::cout << "fps:" << (int)(1 / deltaTime) << std::endl;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

