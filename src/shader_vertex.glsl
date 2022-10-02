#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float spectral_values[12];

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;

out vec3 gouraud_color;

void main()
{
    // A variável gl_Position define a posição final de cada vértice
    // OBRIGATORIAMENTE em "normalized device coordinates" (NDC), onde cada
    // coeficiente estará entre -1 e 1 após divisão por w.
    // Veja {+NDC2+}.
    //
    // O código em "main.cpp" define os vértices dos modelos em coordenadas
    // locais de cada modelo (array model_coefficients). Abaixo, utilizamos
    // operações de modelagem, definição da câmera, e projeção, para computar
    // as coordenadas finais em NDC (variável gl_Position). Após a execução
    // deste Vertex Shader, a placa de vídeo (GPU) fará a divisão por W. Veja
    // slides 41-67 e 69-86 do documento Aula_09_Projecoes.pdf.

    gl_Position = projection * view * model * model_coefficients;

    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);

    vec4 camera_position = inverse(view) * origin;

    // Como as variáveis acima  (tipo vec4) são vetores com 4 coeficientes,
    // também é possível acessar e modificar cada coeficiente de maneira
    // independente. Esses são indexados pelos nomes x, y, z, e w (nessa
    // ordem, isto é, 'x' é o primeiro coeficiente, 'y' é o segundo, ...):
    //
    //     gl_Position.x = model_coefficients.x;
    //     gl_Position.y = model_coefficients.y;
    //     gl_Position.z = model_coefficients.z;
    //     gl_Position.w = model_coefficients.w;
    //

    // Agora definimos outros atributos dos vértices que serão interpolados pelo
    // rasterizador para gerar atributos únicos para cada fragmento gerado.

    // Posição do vértice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;

    position_model = model_coefficients;

    // Normal do vértice atual no sistema de coordenadas global (World).
    // Veja slides 123-151 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    vec4 l = normalize(camera_position - position_world);
    vec4 n = normalize(normal);
    float lambert = max(0, dot(n, l));

    vec3 Kd = vec3(spectral_values[0], spectral_values[1], spectral_values[2]); // Refletância difusa
    vec3 Ks = vec3(spectral_values[3], spectral_values[4], spectral_values[5]); // Refletância especular
    vec3 Ka = vec3(spectral_values[6], spectral_values[7], spectral_values[8]); // Refletância ambiente
    float q = spectral_values[9]; // Expoente especular para o modelo de iluminação de Phong

    vec3 I = vec3(1.0, 1.0, 1.0);

    vec4 p = position_world;

    vec3 Ia = vec3(0.2, 0.2, 0.2);

    vec4 v = normalize(camera_position - p);

    vec4 h = normalize(v + l);

    vec3 blinnPhongSpecTerm = vec3(0.0, 0.0, 0.0);

    vec3 lambert_diffuse_term = vec3(0.0,0.0,0.0);

    lambert_diffuse_term = Kd*I* max(0,dot(n,l));

    vec3 ambient_term = vec3(0.0,0.0,0.0);

    ambient_term = Ka*Ia;

    blinnPhongSpecTerm = Ks*I*pow(dot(n, h), q);

    gouraud_color = lambert_diffuse_term + ambient_term + blinnPhongSpecTerm;

    gouraud_color = pow(gouraud_color, vec3(1.0,1.0,1.0)/2.2);

    //gouraud_color = vec3(0.0, 0.0, 1.0);

    texcoords = texture_coefficients;
}

