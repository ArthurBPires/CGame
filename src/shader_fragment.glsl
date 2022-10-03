#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

in vec4 position_model;

in vec2 texcoords;

in vec3 gouraud_color;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
uniform int object_id;
uniform float spectral_values[12];

uniform vec4 bbox_min;
uniform vec4 bbox_max;

uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    //Carrega o tipo de iluminação do objeto
    //1.0 => Blinn-Phong; 2.0 Phong
    float illumModel = spectral_values[10];
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;
    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));
    l = normalize(camera_position - p);
    //vec4(1.0,1.0,0.5,0.0)
    //l = normalize(vec4(0.0,2.0,1.0,1.0)-p);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    //Definição do half-vector.
    vec4 h = normalize(v + l);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = vec4(0.0,0.0,0.0,0.0); // PREENCHA AQUI o vetor de reflexão especular ideal

    r = -l + 2*n*(dot(n,l));

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd = vec3(spectral_values[0], spectral_values[1], spectral_values[2]); // Refletância difusa
    vec3 Ks = vec3(spectral_values[3], spectral_values[4], spectral_values[5]); // Refletância especular
    vec3 Ka = vec3(spectral_values[6], spectral_values[7], spectral_values[8]); // Refletância ambiente
    float q = spectral_values[9]; // Expoente especular para o modelo de iluminação de Phong

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0); // PREENCH AQUI o espectro da fonte de luz

    /*
    if(dot(normalize(p - (vec4(0.0,2.0,1.0,1.0))),normalize(vec4(0.0,-1.0,0.0,0.0))) < cos(0.523599))
        I = vec3(0.0, 0.0, 0.0);
    */

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2, 0.2, 0.2); // PREENCHA AQUI o espectro da luz ambiente

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = vec3(0.0,0.0,0.0); // PREENCHA AQUI o termo difuso de Lambert
    lambert_diffuse_term = Kd*I* max(0,dot(n,l));

    // Termo ambiente
    vec3 ambient_term = vec3(0.0,0.0,0.0); // PREENCHA AQUI o termo ambiente
    ambient_term = Ka*Ia;

    float U = 0.0;
    float V = 0.0;

    if(illumModel == 1.0){
        if(object_id == 0 /*|| object_id == 2*/){
            vec3 blinnPhongSpecTerm = vec3(0.0, 0.0, 0.0);
            blinnPhongSpecTerm = Ks*I*pow(dot(n, h), q);

            color.a = 1;

            color.rgb = lambert_diffuse_term + ambient_term + blinnPhongSpecTerm;

            color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
            //color.rgb = vec3(0.0, 0.0, 1.0);
        }
        else if(object_id == 1){
            float minx = bbox_min.x;
            float maxx = bbox_max.x;

            float miny = bbox_min.y;
            float maxy = bbox_max.y;

            float minz = bbox_min.z;
            float maxz = bbox_max.z;

            U = (position_model.x-minx)/(maxx-minx);
            V = (position_model.y-miny)/(maxy-miny);

            vec3 Kd0 = texture(TextureImage0, vec2(U,V)).rgb;

            float lambert = max(0,dot(n,l));

            color.rgb = Kd0 * (lambert + 0.01);

            color.a = 1;

            color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
            //color.rgb = vec3(1.0, 1.0, 0.0);
        }
        else if(object_id == 3){
            U = texcoords.x;
            V = texcoords.y;

            vec3 Kd0 = texture(TextureImage1, vec2(U,V)).rgb;

            float lambert = max(0,dot(n,l));

            color.rgb = Kd0 * (lambert + 0.01);

            color.a = 1;

            color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
        }
        else{
            color.a = 1;
            color.rgb = gouraud_color;
        }
    }
    else{
        //if(object_id == 0){
            // Termo especular utilizando o modelo de iluminação de Phong
            vec3 phong_specular_term  = vec3(0.0,0.0,0.0); // PREENCH AQUI o termo especular de Phong
            phong_specular_term = Ks*I* pow(max(0,dot(r,v)),q);

            // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
            // necessário:
            // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
            //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
            //      glEnable(GL_BLEND);
            //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
            //    todos os objetos opacos; e
            // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
            //    suas distâncias para a câmera (desenhando primeiro objetos
            //    transparentes que estão mais longe da câmera).
            // Alpha default = 1 = 100% opaco = 0% transparente
            color.a = 1;

            // Cor final do fragmento calculada com uma combinação dos termos difuso,
            // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
            color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

            // Cor final com correção gamma, considerando monitor sRGB.
            // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
            color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
            //color.rgb = vec3(0.0, 1.0, 0.0);
        //}
        /*else{
            vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

            float ro = 1.0f;

            float norm_p = length(position_model - bbox_center);

            vec4 p_line = bbox_center +
            ro*(position_model-bbox_center)/norm_p;

            vec4 p_vec = p_line - bbox_center;

            float theta = atan(p_vec.x, p_vec.z);
            float phi = asin(p_vec.y/ro);

            U = (theta + M_PI)/(2*M_PI);
            V = (phi + (M_PI/2))/M_PI;

            vec3 Kd0 = texture(TextureImage0, vec2(U,V)).rgb;

            float lambert = max(0,dot(n,l));

            color.rgb = Kd0 * (lambert + 0.01);

            color.a = 1;

            color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
            //color.rgb = vec3(1.0, 0.0, 0.0);
        }*/
    }
}

