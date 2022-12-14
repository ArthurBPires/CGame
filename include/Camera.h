#ifndef CAMERA_H
#define CAMERA_H

#include "Dyn_Object.h"
#include <ostream>
#include <chrono>
#include <thread>
#include <mutex>

enum CameraType {LOOKAT,ISOMETRIC,FREE};

class Camera : public Dyn_Object
{
    public:
        std::mutex m;
        CameraType type = ISOMETRIC;
        float r = 3.5f;
        float nearplane = -1.0f;
        float farplane = -20.0f;
        glm::vec4 * lookat = new glm::vec4(0.0f,0.0f,0.0f,1.0f);
        glm::mat4 view = Matrix_Identity();
        glm::mat4 projection = Matrix_Identity();

        glm::vec4 view_vector = glm::vec4(1.0f,0.0f,0.0f,0.0f);
        glm::vec4 up_vector = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

        bool inAnimation = false;

        Camera(vec4 pos = vec4(0.0,0.0,0.0,1.0), float farplane = -20.0f, float nearplane = -1.0f, float r = 3.5f) :
            Dyn_Object(pos), r(r), nearplane(nearplane), farplane(farplane) {};

        void draw();

        ~Camera();

    protected:

    private:
};
//When implemented, should be divided by TIME_STEP
void Camera::draw()
{
    // Computamos a posição da câmera utilizando coordenadas esféricas.  As
    // variáveis g_CameraDistance, g_CameraPhi, e g_CameraTheta são
    // controladas pelo mouse do usuário. Veja as funções CursorPosCallback()
    // e ScrollCallback().

    // Computamos a matriz "View" utilizando os parâmetros da câmera para
    // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    //m.lock();
    vec4 lookAtOffset = *lookat;
    lookAtOffset.y = 1.0f;

    if(!inAnimation)
    {

        if(type == LOOKAT)
        {
            pos.x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
            pos.y = r*sin(g_CameraPhi);
            pos.z = r*cos(g_CameraPhi)*cos(g_CameraTheta);

            // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
            // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
            view_vector = *lookat - pos; // Vetor "view", sentido para onde a câmera está virada
        }
        else if(type == ISOMETRIC)
        {
            pos.x = lookat->x;
            pos.y = lookat->y + 15.0;
            pos.z = lookat->z + 3.0;

            // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
            // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
            view_vector = *lookat - pos; // Vetor "view", sentido para onde a câmera está virada
        }
        else if(type == FREE)
        {
            pos = lookAtOffset;

            // Vetor "view", sentido para onde a câmera está virada
            view_vector.z = -cos(g_CameraPhi)*cos(g_CameraTheta);
            view_vector.y = -sin(g_CameraPhi);
            view_vector.x = -cos(g_CameraPhi)*sin(g_CameraTheta);
        }
    }
    else if(norm(pos - lookAtOffset) > 0.0005) //if camera in animation, look at player. (norm test to solve runtime error when *(camera->lookat) == camera->pos)
    {
        view_vector = lookAtOffset - pos;
    }

    view = Matrix_Camera_View(pos, view_vector, up_vector);
    //m.unlock();

    if (g_UsePerspectiveProjection)
    {
        // Projeção Perspectiva.
        // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
    }
    else
    {
        // Projeção Ortográfica.
        // Para definição dos valores l, r, b, t ("left", "right", "bottom", "top"),
        // PARA PROJEÇÃO ORTOGRÁFICA veja slides 219-224 do documento Aula_09_Projecoes.pdf.
        // Para simular um "zoom" ortográfico, computamos o valor de "t"
        // utilizando a variável g_CameraDistance.
        float t = 1.5f*g_CameraDistance/2.5f;
        float b = -t;
        float r = t*g_ScreenRatio;
        float l = -r;
        projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
    }

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo
    // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
    // efetivamente aplicadas em todos os pontos.
    glUniformMatrix4fv(view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));
}

Camera::~Camera() {};

#endif // CAMERA_H

