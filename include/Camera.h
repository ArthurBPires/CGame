#ifndef CAMERA_H
#define CAMERA_H

#include "Dyn_Object.h"
#include <ostream>
#include <chrono>
#include <thread>

void bezierCurve(int duration, int frames, std::vector<vec4> points, vec4 * position)
{
    int delay = std::round(duration / frames);

    float step = (float)(1.0f / frames);

    for(float t = 0.0f; t < 1.0f - 0.0001; t += step)
    {
        std::vector<vec4> pointsAux = points;
        do
        {
            int degree = pointsAux.size() - 1;
            for(int i = 0; i < degree; i++)
            {
                vec4 a = pointsAux.front();
                pointsAux.erase(pointsAux.begin());
                vec4 b = pointsAux.front();
                vec4 c = a + t * (b - a);
                pointsAux.push_back(c);
            }
            pointsAux.erase(pointsAux.begin());

            //printf("size after: %d\n", points.size());
            /*
            if(points.size() > 1)
                points.erase(points.begin(),points.begin() + degree + 1);*/
        } while(pointsAux.size() > 1);

        vec4 pointF = pointsAux.front();
        //printf("%f %f %f %f\n", pointF.x,pointF.y,pointF.z,pointF.w);
        position->x = pointF.x;
        position->y = pointF.y;
        position->z = pointF.z;
        //printf("after\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    //printf("end\n");
}

enum CameraType {LOOKAT,ISOMETRIC,FREE};

class Camera : public Dyn_Object
{
    public:
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

    view = Matrix_Camera_View(pos, view_vector, up_vector);

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

