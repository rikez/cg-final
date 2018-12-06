#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube
{
public:
    Cube(GLfloat size): size(size) {}
    
    
    void Draw(GLfloat vertices[216])
    {
        GLfloat half = this->size/2.0f;
        vertices = {                       // Normals for the lights
            -half, -half, -half,  0.0f,  0.0f, -1.0f,
            half, -half, -half,  0.0f,  0.0f, -1.0f,
            half,  half, -half,  0.0f,  0.0f, -1.0f,
            half,  half, -half,  0.0f,  0.0f, -1.0f,
            -half,  half, -half,  0.0f,  0.0f, -1.0f,
            -half, -half, -half,  0.0f,  0.0f, -1.0f,
            
            -half, -half,  half,  0.0f,  0.0f,  1.0f,
            half, -half,  half,  0.0f,  0.0f,  1.0f,
            half,  half,  half,  0.0f,  0.0f,  1.0f,
            half,  half,  half,  0.0f,  0.0f,  1.0f,
            -half,  half,  half,  0.0f,  0.0f,  1.0f,
            -half, -half,  half,  0.0f,  0.0f,  1.0f,
            
            -half,  half,  half, -1.0f,  0.0f,  0.0f,
            -half,  half, -half, -1.0f,  0.0f,  0.0f,
            -half, -half, -half, -1.0f,  0.0f,  0.0f,
            -half, -half, -half, -1.0f,  0.0f,  0.0f,
            -half, -half,  half, -1.0f,  0.0f,  0.0f,
            -half,  half,  half, -1.0f,  0.0f,  0.0f,
            
            half,  half,  half,  1.0f,  0.0f,  0.0f,
            half,  half, -half,  1.0f,  0.0f,  0.0f,
            half, -half, -half,  1.0f,  0.0f,  0.0f,
            half, -half, -half,  1.0f,  0.0f,  0.0f,
            half, -half,  half,  1.0f,  0.0f,  0.0f,
            half,  half,  half,  1.0f,  0.0f,  0.0f,
            
            -half, -half, -half,  0.0f, -1.0f,  0.0f,
            half, -half, -half,  0.0f, -1.0f,  0.0f,
            half, -half,  half,  0.0f, -1.0f,  0.0f,
            half, -half,  half,  0.0f, -1.0f,  0.0f,
            -half, -half,  half,  0.0f, -1.0f,  0.0f,
            -half, -half, -half,  0.0f, -1.0f,  0.0f,
            
            -half,  half, -half,  0.0f,  1.0f,  0.0f,
            half,  half, -half,  0.0f,  1.0f,  0.0f,
            half,  half,  half,  0.0f,  1.0f,  0.0f,
            half,  half,  half,  0.0f,  1.0f,  0.0f,
            -half,  half,  half,  0.0f,  1.0f,  0.0f,
            -half,  half, -half,  0.0f,  1.0f,  0.0f
        };
    }
    
private:
    GLfloat size;
};
