#include <iostream>
#define GLEW_STATIC // Defining a static window

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM Lib for complex matrix calculations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Image library responsible for uploading the textures.
#include "SOIL2/SOIL2.h"

// Shader files reader and compiler
#include "Shader.h"



// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main( )
{
    // Init GLFW
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    glEnable(GL_DEPTH_TEST);
    
    // enable alpha support
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    // Build and compile our shader program
    Shader ourShader( "resources/shaders/core.vs", "resources/shaders/core.frag" );
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    // use with Orthographic Projection
    /*
     GLfloat vertices[] = {
     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,
     0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 0.0f,
     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
     -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,
     
     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
     0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
     -0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 1.0f,
     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
     
     -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     -0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
     -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     
     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
     0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     
     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
     0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     
     -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
     -0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
     -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f
     };
     */
    
    // use with Perspective Projection
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3     * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind VAO
    
    // Load and create a texture
    GLuint texture;
    
    int width, height;
    
    
    // ===================
    // Texture
    // ===================
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    unsigned char *image = SOIL_load_image("resources/images/FEI.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    
    glm::mat4 projection;
    projection = glm::perspective(45.0f, (GLfloat)screenWidth/ (GLfloat)screenHeight, .1f, 1000.0f);
    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture" ), 0);
        
        // Draw the triangle
        ourShader.Use();
        
        glm::mat4 model;
        glm::mat4 view;
        model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f , glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        GLint modelLocation = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLocation = glGetUniformLocation(ourShader.Program, "view");
        GLint projectionLocation = glGetUniformLocation(ourShader.Program, "projection");
        
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
       
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}
