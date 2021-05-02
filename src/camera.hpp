#pragma once

#include <iostream>

#include "vec3.hpp"
#include "matrix.hpp"
#include "gtc/matrix_transform.hpp"
#include "ext.hpp"

namespace camera
{
    enum class Movement { // define movement direction
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // some initial values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
}


class Camera{
public:
    /**
     * construct a camera with some basic paramaters.
     * @param _position the position of camera
     * @param _up the up vector in the world
     * @param _zoom the field of view
     * @param _pitch the degree of pitch rotate
     * @param _yaw the degree of yaw rotate
    */
    Camera(
        const glm::vec3& _position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& _up = glm::vec3(0.0f, 1.0f, 0.0f),
        const float& _zoom = camera::ZOOM,
        const float& _pitch = camera::PITCH,
        const float& _yaw = camera::YAW)
        :position(_position), front(glm::vec3(0.0, 0.0, -1.0f)), worldUp(_up), zoom(_zoom), pitch(_pitch), yaw(_yaw), movementSensitivity(camera::SENSITIVITY), movementSpeed(camera::SPEED)
    {
        updateCameraVectors();
    }
    ~Camera(){}

    /**
     * process camera movement
     * @param direction the direction the camera move
     * @param deltaTime the delta time of current frame and last frame
    */
    void processMovement(camera::Movement direction, float deltaTime) {
        float speed = movementSpeed * deltaTime;
        switch (direction)
        {
        case camera::Movement::FORWARD:
            position += speed * front;
            break;
        
        case camera::Movement::BACKWARD:
            position -= speed * front;
            break;
        
        case camera::Movement::LEFT:
            position -= speed * right;
            break;
         
         case camera::Movement::RIGHT:
            position += speed * right;
            break;
        }
    }

    /**
     * process camera look around
     * @param xOffset the x-Axis offset of mouse
     * @param yOffset the y-Axis offset of mouse
     * @param constrainPitch whether constrain pitch in -90 to 90 degree.
    */
    void processLookAround(float xOffset, float yOffset, bool constrainPitch) {
        xOffset *= movementSensitivity;
        yOffset *= movementSensitivity;
        
        yaw += xOffset;
        pitch += yOffset;

        if(constrainPitch) {
            if(pitch > 89.0)
                pitch = 89.0;
            if(pitch < -89.0)
                pitch = -89.0;
        }
        updateCameraVectors();
    }

    /**
     * process camera zoom, it use scoll y-Axis offset
     * @param yOffset the y-Axis offset of mouse.
    */
    void processZoom(float yOffset) {
        zoom -= yOffset;
        if(zoom < 1.0f)
            zoom = 1.0f;
        if(zoom > 45.0f)
            zoom = 45.0f;
    }

    glm::mat4 getViewMat() { // get view matrix
        return glm::lookAt(position, position + front, up);
    }

    inline float getZoom() {
        return zoom;
    }

    inline glm::vec3 getPosition() {
        return position;
    }

    inline glm::vec3 getFront() {
        return front;
    }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        direction.y = std::sin(glm::radians(pitch));
        direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    glm::vec3 position; // the camera postion
    glm::vec3 front; // the camera move forward vector
    glm::vec3 worldUp; // the world up vector
    glm::vec3 up; // the camera up vector
    glm::vec3 right; // the camera right vector

    float pitch;
    float yaw;

    float zoom;
    float movementSpeed;
    float movementSensitivity;
};