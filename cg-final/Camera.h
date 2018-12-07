#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Definindo as possíveis direções do movimento da camera
 */
enum MovimentosCamera
{
    FRENTE,
    TRAS,
    ESQUERDA,
    DIREITA
};

/**
 * Valores padrão da câmera
 */
const GLfloat MOVIMENTOZ      = -90.0f;
const GLfloat MOVIMENTOX      =  0.0f;
const GLfloat VELOCIDADE      =  6.0f;
const GLfloat SENSIBILIDADE   =  0.25f;
const GLfloat ZOOM            =  45.0f;


/**
 * Abstração de uma camera que processa a entrada do teclado e mouse e calcular
 * os ângulos, vetores e matrizes.
 */
class Camera
{
public:
    Camera( glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            GLfloat movZ = MOVIMENTOZ,
            GLfloat movX = MOVIMENTOX ) :
                frontal( glm::vec3(0.0f, 0.0f, -1.0f)),
                velocidadeMovimento(VELOCIDADE),
                sensibilidadeMouse(SENSIBILIDADE),
                zoom(ZOOM)
    {
        this->posicao = posicao;
        this->acima = cima;
        this->movZ = movZ;
        this->movX = movX;
        this->atualizaVetoresDaCamera( );
    }
    
    Camera( GLfloat posX,
            GLfloat posY,
            GLfloat posZ,
            GLfloat cimaX,
            GLfloat cimaY,
            GLfloat cimaZ,
            GLfloat movZ,
            GLfloat movX ) :
                frontal(glm::vec3( 0.0f, 0.0f, -1.0f)),
                velocidadeMovimento(VELOCIDADE),
                sensibilidadeMouse(SENSIBILIDADE),
                zoom(ZOOM)
    {
        this->posicao = glm::vec3(posX, posY, posZ);
        this->acima = glm::vec3(cimaX, cimaY, cimaZ);
        this->movZ = movZ;
        this->movX = movX;
        this->atualizaVetoresDaCamera( );
    }
    
    // Retorna matriz de visualizacao usando lookAt
    glm::mat4 ObterMatrizVisualizacao()
    {
        return glm::lookAt(this->posicao, this->posicao + this->frontal, this->cima);
    }
    
    void ProcessaTeclado(MovimentosCamera direcao, GLfloat deltaT)
    {
        GLfloat velocidade = this->velocidadeMovimento * deltaT;
        
        if (direcao == FRENTE)
        {
            this->posicao += this->frontal * velocidade;
        }
        
        if (direcao == TRAS)
        {
            this->posicao -= this->frontal * velocidade;
        }
        
        if (direcao == ESQUERDA)
        {
            this->posicao -= this->direita * velocidade;
        }
        
        if (direcao == DIREITA)
        {
            this->posicao += this->direita * velocidade;
        }
    }
    
    void ProcessaMovimentoMouse(GLfloat deltaX, GLfloat deltaY, GLboolean limiteMovX = true)
    {
        deltaX *= this->sensibilidadeMouse;
        deltaY *= this->sensibilidadeMouse;
        
        this->movZ += deltaX;
        this->movX += deltaY;
        
        if (limiteMovX)
        {
            if ( this->movX > 89.0f )
            {
                this->movX = 89.0f;
            }
            
            if ( this->movX < -89.0f )
            {
                this->movX = -89.0f;
            }
        }
        
        this->atualizaVetoresDaCamera( );
    }
    
    void ProcessMouseScroll( GLfloat yOffset )
    {
        
    }
    
    GLfloat ObterZoom( )
    {
        return this->zoom;
    }
    
    glm::vec3 ObterPosicao( )
    {
        return this->posicao;
    }
    
    glm::vec3 ObterFrente( )
    {
        return this->frontal;
    }
    
private:
    // Atributos da camera
    glm::vec3 posicao;
    glm::vec3 frontal;
    glm::vec3 cima;
    glm::vec3 direita;
    glm::vec3 acima;
    
    // Angulos
    GLfloat movZ;
    GLfloat movX;
    
    // Opcoes
    GLfloat velocidadeMovimento;
    GLfloat sensibilidadeMouse;
    GLfloat zoom;
    
    // Calcula os vetores frontais da camera
    void atualizaVetoresDaCamera( )
    {
        // Calcula vetor frontal
        glm::vec3 frontal;
        frontal.x = cos(glm::radians( this->movZ ) ) * cos( glm::radians( this->movX ) );
        frontal.y = sin(glm::radians( this->movX ) );
        frontal.z = sin(glm::radians( this->movZ ) ) * cos( glm::radians( this->movX ) );
        this->frontal = glm::normalize(frontal);
        
        // Recalcula os vetores da direita e de cima.
        this->direita = glm::normalize(glm::cross(this->frontal, this->acima));
        this->cima = glm::normalize(glm::cross(this->direita, this->frontal));
    }
};
