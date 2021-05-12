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

Camera _camera(glm::vec3(0.0f, 20.0f, 155.0f));


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
    
    Shader planet_shader("../shader/model/vertex.glsl", "../shader/model/fragment.glsl");
    Model::Model planetModel("../res/model/planet/planet.obj");
    Shader rock_shader("../shader/rock/vertex.glsl", "../shader/rock/fragment.glsl");
    Model::Model rockModel("../res/model/rock/rock.obj");

    int amount = 30000;
    std::vector<glm::mat4> rockModelMatrices;
    rockModelMatrices.reserve(amount);
    std::srand(glfwGetTime());
    float radius = 150.0f;
    float offset = 25.0f;
    for(int i = 0; i < amount; i++) {
        glm::mat4 m = glm::mat4(1.0);
        float angle = (float) i / amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = std::sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = std::cos(angle) * radius + displacement;
        m = glm::translate(m, glm::vec3(x, y, z));

        float scale = (rand() % 20) / 100.0f + 0.05f;
        m = glm::scale(m, glm::vec3(scale));

        float rotateAngle = (rand() % 360);
        m = glm::rotate(m, rotateAngle, glm::vec3(0.4, 0.6, 0.8));
        rockModelMatrices.push_back(m);
    }

    std::vector<Model::Mesh> rockMeshs = rockModel.getMesh();
    GLuint rockBuffer;
    glGenBuffers(1, &rockBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rockBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &rockModelMatrices[0], GL_STATIC_DRAW);

    for(auto const& mesh: rockMeshs) {
        GLuint VAO = mesh.getVAO();
        glBindVertexArray(VAO);
        std::size_t vec4Size = sizeof(glm::vec4);
        int beginPos = 3;
        for(int j = 0; j < 4; j++) {
            glVertexAttribPointer(beginPos + j, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(j * sizeof(glm::vec4)));
            glEnableVertexAttribArray(beginPos + j);
            glVertexAttribDivisor(beginPos + j, 1);
        }
        glBindVertexArray(0);
    }
    
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
        projection = glm::perspective(glm::radians(_camera.getZoom()), (float)width/height, 0.1f, 1000.0f);


        planet_shader.use();
        model = glm::translate(model, glm::vec3(0.0, -3.0, 0.0));
        model = glm::scale(model, glm::vec3(4.0));
        planet_shader.setMat4("model", model);
        planet_shader.setMat4("view", view);
        planet_shader.setMat4("projection", projection);
        planetModel.draw(planet_shader);
        
        rock_shader.use();
        rock_shader.setMat4("view", view);
        rock_shader.setMat4("projection", projection);
        
        for(const auto& mesh: rockMeshs) {
            mesh.bindTexture(rock_shader);
            glBindVertexArray(mesh.getVAO());
            glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::cout << "fps:" << (int)(1 / deltaTime) << std::endl;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}