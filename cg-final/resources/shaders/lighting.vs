#version 330 core
layout (location = 0) in vec3 posicao;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 coordenadasTextura;

out vec3 Normal;
out vec3 PosicaoFragmento;
out vec2 CoordenadasTextura;

uniform mat4 modelo;
uniform mat4 visualizacao;
uniform mat4 projecao;

void main()
{
    gl_Position = projecao * visualizacao * modelo * vec4(posicao, 1.0f);
    PosicaoFragmento = vec3(modelo * vec4(posicao, 1.0f));
    Normal = mat3(transpose(inverse(modelo))) * normal;
    CoordenadasTextura = coordenadasTextura;
}
