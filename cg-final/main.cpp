#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Para aplicar texturas
#include "SOIL2/SOIL2.h"

// GLM Para calculos de vetores e matrizes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Internas
#include "Shader.h"
#include "Camera.h"

// Callbacks de mouse e teclado
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow *window, double posX, double posY);
void move();

// Dimensoes da janela
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera - Variáveis globais
Camera  camera(glm::vec3( 0.0f, 0.0f, 3.0f ));
GLfloat xAnterior = WIDTH / 2.0;
GLfloat yAnterior = HEIGHT / 2.0;
bool teclas[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 posicaoDaLuz(1.2f, 1.0f, 2.0f);

// DeltaT
GLfloat deltaT = 0.0f; // Tempo decorrido entre o FRAME atual e o anterior.
GLfloat frameAnterior = 0.0f;      // Time of last frame

int main( )
{
    // Inicializa opengl
    glfwInit( );
    
    // Seta as opções necessárias do opengl
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Cria a Janela
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Combinação das Luzes", nullptr, nullptr);
    
    if ( nullptr == window )
    {
        std::cout << "FALHA AO CRIAR JANELA" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    
    // Registra as funções de Callback
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    
    // Desabilitabdo o cursor na janela
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "FALHA AO INICIALIZAR A BIBLIOTECA GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define as dimensões da viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Habilita 3D
    glEnable(GL_DEPTH_TEST);
    
    
    // Compila os shader files
    Shader iluminacaoShader( "resources/shaders/lighting.vs", "resources/shaders/lighting.frag" );
    Shader lampadaShader( "resources/shaders/lamp.vs", "resources/shaders/lamp.frag" );
    
    // Cria os os vertices do cubo, adicona as normais para iluminação e as coordenadas de textura
    GLfloat vertices[] =
    {
        // POSICOES            // NORMAL                // TEXTURA
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,      1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
    };
    
    // Positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(  0.0f,   0.0f,   0.0f    ),
        glm::vec3(  2.0f,   5.0f,   -15.0f  ),
        glm::vec3(  -1.5f,  -2.2f,  -2.5f   ),
        glm::vec3(  -3.8f,  -2.0f,  -12.3f  ),
        glm::vec3(  2.4f,   -0.4f,  -3.5f   ),
        glm::vec3(  -1.7f,  3.0f,   -7.5f   ),
        glm::vec3(  1.3f,   -2.0f,  -2.5f   ),
        glm::vec3(  1.5f,   2.0f,   -2.5f   ),
        glm::vec3(  1.5f,   0.2f,   -1.5f   ),
        glm::vec3(  -1.3f,  1.0f,   -1.5f   )
    };
    
    // Positions of the point lights
    glm::vec3 posLuzesPontiforme[] = {
        glm::vec3(  0.7f,  0.2f,  2.0f      ),
        glm::vec3(  2.3f, -3.3f, -4.0f      ),
        glm::vec3(  -4.0f,  2.0f, -12.0f    ),
        glm::vec3(  0.0f,  0.0f, -3.0f      )
    };
    
    // Seta os container de buffer VAO VBO
    GLuint ObjetoBuffer, atributosCubo;
    glGenVertexArrays(1, &atributosCubo);
    glGenBuffers(1, &ObjetoBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, ObjetoBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(atributosCubo);
    
    // Setando as posições no buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    
    // Setando as normais no buffer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // Setando a textura no buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    // Setando container do cubo de luz
    GLuint cuboLuz;
    glGenVertexArrays(1, &cuboLuz);
    glBindVertexArray(cuboLuz);
    glBindBuffer(GL_ARRAY_BUFFER, ObjetoBuffer);

    // Não há necessidade de se utilizar normais e textura na lampada, somente jogamos as posições no buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    // Carregando as texturas
    GLuint mapaIlumDifusora, mapaIlumEspecular, mapaIlumEmissora;
    glGenTextures(1, &mapaIlumDifusora);
    glGenTextures(1, &mapaIlumEspecular);
    glGenTextures(1, &mapaIlumEmissora);
    
    int larguraImagem, alturaImagem;
    unsigned char *imagem;
    
    // Mapa da Ilum Difusora
    imagem = SOIL_load_image("resources/images/container2.png", &larguraImagem, &alturaImagem, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, mapaIlumDifusora);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, larguraImagem, alturaImagem, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(imagem);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    // Mapa Ilum Especular
    imagem = SOIL_load_image("resources/images/container2_specular.png", &larguraImagem, &alturaImagem, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, mapaIlumEspecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, larguraImagem, alturaImagem, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(imagem);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    // Setando os pixels de textura
    iluminacaoShader.Use( );
    glUniform1i(glGetUniformLocation(iluminacaoShader.Program, "material.difusa"), 0);
    glUniform1i(glGetUniformLocation(iluminacaoShader.Program, "material.especular"), 1);
    
    // Criando matrix de perspective
    glm::mat4 projecao = glm::perspective(camera.ObterZoom(), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        // Calcula deltaT do frameatual
        GLfloat frameAtual = glfwGetTime( );
        deltaT = frameAtual - frameAnterior;
        frameAnterior = frameAtual;
        
        // Checa se algum evento foi ativado(teclado, mouse), se sim chama os callbacks.
        glfwPollEvents( );
        move();
        
        // Clear the colorbuffer
        glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // Utilizando o shader de iluminação para desenhar os objetos
        iluminacaoShader.Use();
        GLint locPosicaoDaVisualizacao = glGetUniformLocation(iluminacaoShader.Program, "posicaoVisualizacao");
        glUniform3f(locPosicaoDaVisualizacao, camera.ObterPosicao().x, camera.ObterPosicao().y, camera.ObterPosicao().z);
        
        // Seta as propriedades do material
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "material.brilho"), 32.0f);
        
        // == ==========================
        // Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
        // the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
        // by defining light types as classes and set their values in there, or by using a more efficient uniform approach
        // by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
        // == ==========================
        
        // Luz Direcional
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzDirecional.direcao"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzDirecional.ambiente"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzDirecional.difusa"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzDirecional.especular"), 0.5f, 0.5f, 0.5f);
        
        // Luz Pontiforme 1
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].posicao"), posLuzesPontiforme[0].x, posLuzesPontiforme[0].y, posLuzesPontiforme[0].z);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].ambiente"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].difusa"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].especular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].constante"), 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[0].quadratico"), 0.032f);
        
        // Luz Pontiforme 2
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].posicao"), posLuzesPontiforme[1].x, posLuzesPontiforme[1].y, posLuzesPontiforme[1].z);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].ambiente"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].difusa"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].especular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].constante"), 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[1].quadratico"), 0.032f);
        
        
        // Luz Pontiforme 3
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].posicao"), posLuzesPontiforme[2].x, posLuzesPontiforme[2].y, posLuzesPontiforme[2].z);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].ambiente"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].difusa"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].especular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].constante"), 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[2].quadratico"), 0.032f);
        
        
        // Luz Pontiforme 4
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].posicao"), posLuzesPontiforme[3].x, posLuzesPontiforme[3].y, posLuzesPontiforme[3].z);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].ambiente"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].difusa"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].especular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].constante"), 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "luzesPontiforme[3].quadratico"), 0.032f);
        
        // Holofote
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "holofote.posicao"), camera.ObterPosicao().x, camera.ObterPosicao().y, camera.ObterPosicao().z);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "holofote.direcao"), camera.ObterFrente().x, camera.ObterFrente().y, camera.ObterFrente().z);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "holofote.ambiente"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "holofote.difusa"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(iluminacaoShader.Program, "holofote.especular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "holofote.constante"), 1.0f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "holofote.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "holofote.quadratico"), 0.032f);
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "holofote.pontoLimitante"), glm::cos(glm::radians( 12.5f )));
        glUniform1f(glGetUniformLocation(iluminacaoShader.Program, "holofote.pontoLimitanteExterno"), glm::cos(glm::radians(15.0f)));
        
        // Cria as transf geometricas da camera
        glm::mat4 visualizacao;
        visualizacao = camera.ObterMatrizVisualizacao();
        
        // Obtem as localizacoes estabelecidas nos shaders
        GLint localizacaoModelo = glGetUniformLocation(iluminacaoShader.Program, "modelo");
        GLint localizacaoVisualizacao = glGetUniformLocation(iluminacaoShader.Program, "visualizacao");
        GLint localizacaoProjecao = glGetUniformLocation(iluminacaoShader.Program, "projecao");
        
        // Passa as matrizes para o shader
        glUniformMatrix4fv(localizacaoVisualizacao, 1, GL_FALSE, glm::value_ptr(visualizacao));
        glUniformMatrix4fv(localizacaoProjecao, 1, GL_FALSE, glm::value_ptr(projecao));
        
        // Binda mapa de ilum difusora
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mapaIlumDifusora);
        
        // Binda mapa ilum especular
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mapaIlumEspecular);
        
        glm::mat4 modelo;
        glBindVertexArray(atributosCubo);
        for (GLuint i = 0; i < 10; i++)
        {
            modelo = glm::mat4();
            modelo = glm::translate(modelo, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            modelo = glm::rotate(modelo, angle, glm::vec3(1.0f, 0.3f, 0.5f ));
            glUniformMatrix4fv(localizacaoModelo, 1, GL_FALSE, glm::value_ptr(modelo));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray( 0 );
        
        
        // Desenha lampada utilizando shader próprio
        lampadaShader.Use( );
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        // Obtem as localizacoes estabelecidas nos shaders
        localizacaoModelo = glGetUniformLocation(iluminacaoShader.Program, "modelo");
        localizacaoVisualizacao = glGetUniformLocation(iluminacaoShader.Program, "visualizacao");
        localizacaoProjecao = glGetUniformLocation(iluminacaoShader.Program, "projecao");
        
        // Seta as matrizes
        glUniformMatrix4fv(localizacaoVisualizacao, 1, GL_FALSE, glm::value_ptr(visualizacao));
        glUniformMatrix4fv(localizacaoProjecao, 1, GL_FALSE, glm::value_ptr(projecao));
        modelo = glm::mat4();
        modelo = glm::translate(modelo, posicaoDaLuz);
        modelo = glm::scale(modelo, glm::vec3( 0.2f )); // Lampada pequena
        glUniformMatrix4fv(localizacaoModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        
        // Desenha o objeto de luz
        glBindVertexArray(cuboLuz);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray( 0 );
        
        
        // Desenha as luzes pontiformes
        glBindVertexArray(cuboLuz);
        for (GLuint i = 0; i < 4; i++)
        {
            modelo = glm::mat4();
            modelo = glm::translate(modelo, posLuzesPontiforme[i]);
            modelo = glm::scale(modelo, glm::vec3(0.2f)); // Lampada pequena
            glUniformMatrix4fv(localizacaoModelo, 1, GL_FALSE, glm::value_ptr(modelo));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        // Faz o swap dos buffers redesenhando
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &atributosCubo);
    glDeleteVertexArrays(1, &cuboLuz);
    glDeleteBuffers(1, &ObjetoBuffer);
    
    glfwTerminate();
    
    return 0;
}

void move()
{
    if ( teclas[GLFW_KEY_W] || teclas[GLFW_KEY_UP] )
    {
        camera.ProcessaTeclado(FRENTE, deltaT);
    }
    
    if ( teclas[GLFW_KEY_S] || teclas[GLFW_KEY_DOWN] )
    {
        camera.ProcessaTeclado(TRAS, deltaT);
    }
    
    if ( teclas[GLFW_KEY_A] || teclas[GLFW_KEY_LEFT] )
    {
        camera.ProcessaTeclado(ESQUERDA, deltaT);
    }
    
    if ( teclas[GLFW_KEY_D] || teclas[GLFW_KEY_RIGHT] )
    {
        camera.ProcessaTeclado(DIREITA, deltaT);
    }
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            teclas[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            teclas[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow *window, double posX, double posY)
{
    if (firstMouse)
    {
        xAnterior = posX;
        yAnterior = posY;
        firstMouse = false;
    }
    
    GLfloat deltaX = posX - xAnterior;
    GLfloat deltaY = yAnterior - posY;  // Reverte pq a coordenada y vai de baixo para a esquerda
    
    xAnterior = posX;
    yAnterior = posY;
    
    camera.ProcessaMovimentoMouse(deltaX, deltaY);
}
