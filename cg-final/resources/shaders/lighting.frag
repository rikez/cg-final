#version 330 core

// FONTE: http://professores.dcc.ufla.br/~bruno/aulas/cg/monte-mor/53.htm

#define NUMERO_DE_LUZES_PONTIFORMES 4

// Utilizado para aplicar as texturas
struct Material
{
    sampler2D difusa;
    sampler2D especular;
    float brilho;
};

/**
* Directional Lights
* Este tipo de luz é necessário especificar apenas uma direção e partir daí
* os objetos serão iluminados
* de acordo com a mesma
*/
struct LuzDirecional
{
    vec3 direcao;
    
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
};

/**
 * Point Lights
 * Neste tipo de luz basta definir um ponto que a partir
 * desse haverá uma iluminação em todas as direções
 */
struct LuzPontiforme
{
    vec3 posicao;
    
    float constante;
    float linear;
    float quadratico;
    
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
};

/**
 * SpotLight
 * No geral, a luz não é emitida em todas as direções pois objetos
 * que estão atrás não são iluminados. Para definir uma luz
 * como essa é necessário definir a posição e direção da fonte de luz,
 * qual a concentração de luz  e um ângulo que irá indicar
 * qual a área de iluminação
 */
struct Holofote
{
    vec3 posicao;
    vec3 direcao;
    
    float pontoLimitante; // cutOff
    float pontoLimitanteExterno; //outerCuttOff
    
    float constante;
    float linear;
    float quadratico;
    
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
};

in vec3 PosicaoFragmento;
in vec3 Normal;
in vec2 CoordenadasTextura;

out vec4 color;

uniform vec3 posicaoVisualizacao;
uniform LuzDirecional luzDirecional;
uniform LuzPontiforme luzesPontiforme[NUMERO_DE_LUZES_PONTIFORMES];
uniform Holofote holofote;
uniform Material material;

vec3 CalculaLuzDirecional(LuzDirecional luz, vec3 normal, vec3 direcaoDaVisualizacao);
vec3 CalculaLuzPontiforme(LuzPontiforme luz, vec3 normal, vec3 posicaoFragmento, vec3 direcaoDaVisualizacao);
vec3 CalculaHolofote(Holofote luz, vec3 normal, vec3 posicaoFragmento, vec3 direcaoDaVisualizacao);

void main( )
{
    // Normalizando as propriedades
    vec3 normal = normalize(Normal);
    vec3 direcaoDaVisualizacao = normalize(posicaoVisualizacao - PosicaoFragmento);
    
    // Calculando a luz direcional
    vec3 resultado = CalculaLuzDirecional(luzDirecional, normal, direcaoDaVisualizacao);
    
    // Calculando as luzes pontiformes
    for (int i = 0; i < NUMERO_DE_LUZES_PONTIFORMES; i++)
    {
        resultado += CalculaLuzPontiforme(luzesPontiforme[i], normal, PosicaoFragmento, direcaoDaVisualizacao);
    }
    
    // Calculando a luz holofote
    resultado += CalculaHolofote(holofote, normal, PosicaoFragmento, direcaoDaVisualizacao);
    
    color = vec4(resultado, 1.0f);
}

// Método para calcular as cores quando utiliza-se luz direcional
vec3 CalculaLuzDirecional(LuzDirecional luz, vec3 normal, vec3 direcaoDaVisualizacao)
{
    vec3 direcaoLuz = normalize(-luz.direcao);
    
    // Sombreamento causado pela iluminação difusora
    float sombraDifusora = max(dot(normal, direcaoLuz), 0.0);
    
    // Sombreamento causado pela iluminação especular
    vec3 reflexaoDirecional = reflect(-direcaoLuz, normal);
    float sombraEspecular = pow(max(dot(direcaoDaVisualizacao, reflexaoDirecional), 0.0 ), material.brilho);
    
    // Combinando as iluminações: ambiente, difusa e especular
    vec3 ambiente = luz.ambiente * vec3(texture(material.difusa, CoordenadasTextura));
    vec3 difusa = luz.difusa * sombraDifusora * vec3(texture(material.difusa, CoordenadasTextura));
    vec3 especular = luz.especular * sombraEspecular * vec3(texture(material.especular, CoordenadasTextura));
    
    return (ambiente + difusa + especular);
}

// Método para calcular as cores quando utiliza-se luz pontiforme
vec3 CalculaLuzPontiforme(LuzPontiforme luz, vec3 normal, vec3 posicaoFragmento, vec3 direcaoDaVisualizacao)
{
    vec3 direcaoLuz = normalize(luz.posicao - posicaoFragmento);
    
    // Sombreamento causado pela iluminação difusora
    float sombraDifusora = max(dot(normal, direcaoLuz), 0.0);
    
    // Sombreamento causado pela iluminação especular
    vec3 reflexaoDirecional = reflect(-direcaoLuz, normal);
    float sombraEspecular = pow(max(dot(direcaoDaVisualizacao, reflexaoDirecional), 0.0 ), material.brilho);
    
    // Calculando a distancia entre o ponto e o fragmento
    float distancia = length(luz.posicao - posicaoFragmento);
    float reduzindoForcaLuz = 1.0f/(luz.constante + luz.linear * distancia + luz.quadratico * (distancia * distancia));
    
    // Combinando as iluminações: ambiente, difusa e especular
    vec3 ambiente = luz.ambiente * vec3(texture(material.difusa, CoordenadasTextura));
    vec3 difusa = luz.difusa * sombraDifusora * vec3(texture(material.difusa, CoordenadasTextura));
    vec3 especular = luz.especular * sombraEspecular * vec3(texture(material.especular, CoordenadasTextura));
    
    ambiente *= reduzindoForcaLuz;
    difusa *= reduzindoForcaLuz;
    especular *= reduzindoForcaLuz;
    
    return (ambiente + difusa + especular);
}

// Calculates the color when using a spot light.
vec3 CalculaHolofote(Holofote luz, vec3 normal, vec3 posicaoFragmento, vec3 direcaoDaVisualizacao)
{
    vec3 direcaoLuz = normalize(luz.posicao - posicaoFragmento);
    
    // Sombreamento causado pela iluminação difusora
    float sombraDifusora = max(dot(normal, direcaoLuz), 0.0);
    
    // Sombreamento causado pela iluminação especular
    vec3 reflexaoDirecional = reflect(-direcaoLuz, normal);
    float sombraEspecular = pow(max(dot(direcaoDaVisualizacao, reflexaoDirecional), 0.0 ), material.brilho);
    
    // Calculando a distancia entre o ponto e o fragmento
    float distancia = length(luz.posicao - posicaoFragmento);
    float reduzindoForcaLuz = 1.0f/(luz.constante + luz.linear * distancia + luz.quadratico * (distancia * distancia));
    
    
    // Intensidade do holofote
    float theta = dot(direcaoLuz, normalize(-luz.direcao));
    float epsilon = luz.pontoLimitante - luz.pontoLimitanteExterno;
    float intensidade = clamp( (theta - luz.pontoLimitanteExterno) / epsilon, 0.0, 1.0 );
    
    // Combinando as iluminações: ambiente, difusa e especular
    vec3 ambiente = luz.ambiente * vec3(texture(material.difusa, CoordenadasTextura));
    vec3 difusa = luz.difusa * sombraDifusora * vec3(texture(material.difusa, CoordenadasTextura));
    vec3 especular = luz.especular * sombraEspecular * vec3(texture(material.especular, CoordenadasTextura));
    
    ambiente *= reduzindoForcaLuz * intensidade;
    difusa *= reduzindoForcaLuz * intensidade;
    especular *= reduzindoForcaLuz * intensidade;
    
    return (ambiente + difusa + especular);
}
