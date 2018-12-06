//
//  Camera.h
//  cg-final
//
//  Created by Enrico Alvarenga on 2018-12-05.
//  Copyright © 2018 Enrico Alvarenga. All rights reserved.
//
#pragma once

#include <vector>
#define GLEW_STATIC

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FOWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat SENSITIVITY = 0.10f;
const GLfloat ZOOM = 45.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 90.0f;

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH):
            front(glm::vec3(0.0f, 0.0f, -1.0f)),
            movementSpeed(SPEED),
            mouseSensitivity(SENSITIVITY),
            zoom(ZOOM)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch):
        front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY),
        zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt( this->position, this->position + this->front, this->up);
    }
    
    void OnKeyPress(CameraMovement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->movementSpeed * deltaTime;
        
        if (direction == FOWARD)
        {
            this->position += this->front * velocity;
        }
        
        if (direction == BACKWARD)
        {
            this->position -= this->front * velocity;
        }
        
        if (direction == LEFT)
        {
            this->position -= this->right * velocity;
        }
        
        if (direction == RIGHT)
        {
            this->position += this->right * velocity;
        }
    }
    
    void OnMouseMove(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
    {
        xOffset = xOffset * this->mouseSensitivity;
        yOffset = yOffset * this->mouseSensitivity;
        
        this->yaw = this->yaw + xOffset;
        this->pitch = this->pitch + yOffset;
        
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
            {
                this->pitch = 89.0f;
            }
            
            if (this->pitch < -89.0f)
            {
                this->pitch = -89.0f;
            }
            
        }
        this->updateCameraVectors();
    }
    
    void OnMouseScroll(GLfloat yOffset)
    {
        if (this->zoom >= 1.0f && this->zoom <= 45.0f)
        {
            this->zoom -= yOffset;
        }
        
        if (this->zoom <= 1.0f)
        {
            this->zoom = 1.0f;
        }
        
        if (this->zoom >= 45.0f)
        {
            this->zoom = 45.0f;
        }
        
        this->updateCameraVectors();
    }
    
    GLfloat GetZoom() {
        return this->zoom;
    }
    
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    
    void updateCameraVectors()
    {
        glm::vec3 front;
        
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * sin(glm::radians(this->pitch));
        
        // Normalizing the vectors
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
    
};




