#ifndef SCENE_H
#define SCENE_H

#include "Player.h"
#include "Camera.h"
#include "Timer.h"

#include "weapons.h"
#include "collisions.h"


#include <iostream>
#include <string>
#include <vector>

class Enemy;



class Scene
{
    public:
        Scene();
        ~Scene();
        static Camera * camera;
        static Player * player;
        static std::vector<Enemy *> enemies;
        static std::vector<Object *> objects;
        static Book * book;

        static Configuration * config;

        void test();
        void level1();
        void level2();

        //void cameraThread();

        static void renderInit();
        static void renderBaseline();
        static void renderOther();
        static void loadModels();
        static void loadModels(std::vector<std::string> paths);
        static void clearObjects();
        void keyEventHandler();
    protected:

    private:
};
Configuration * Scene::config = &g_config;
Camera * Scene::camera = NULL;
Player * Scene::player = NULL;
Book * Scene::book = NULL;
std::vector<Enemy *> Scene::enemies;
std::vector<Object *> Scene::objects;

Scene::Scene() {srand (static_cast <unsigned> (time(0)));};
Scene::~Scene() {};

std::vector<glm::vec4> bezierCurve(float duration, std::vector<glm::vec4> points)
{
    double frames = duration * Scene::config->framesPerSecond;

    float step = (float)(1.0f / frames);

    std::vector<glm::vec4> posPerFrame;

    for(float t = 0.0f; t < 1.0f; t += step)
    {
        std::vector<glm::vec4> pointsAux = points;
        do
        {
            int degree = pointsAux.size() - 1;
            for(int i = 0; i < degree; i++)
            {
                glm::vec4 a = pointsAux.front();
                pointsAux.erase(pointsAux.begin());
                glm::vec4 b = pointsAux.front();
                glm::vec4 c = a + t * (b - a);
                pointsAux.push_back(c);
            }
            pointsAux.erase(pointsAux.begin());
        } while(pointsAux.size() > 1);

        posPerFrame.push_back(pointsAux.front());
    }

    return posPerFrame;
}

void cameraZoomIn()
{
    Scene::camera->inAnimation = true;
    std::vector<vec4> points = std::vector<vec4>();
    points.push_back(Scene::camera->pos);
    points.push_back(glm::vec4(Scene::camera->pos.x,1.0f,(Scene::camera->pos.z + 3.0),1.0));
    points.push_back(vec4(Scene::player->pos.x,1.0f,Scene::player->pos.z,1.0));
    vec4 endPoint = Scene::player->pos;
    endPoint.y = 1.0f;
    points.push_back(endPoint);

	std::vector<glm::vec4> anim = bezierCurve(0.5,points);

	double nowTime = 0, deltaTime = 0, lastTime = glfwGetTime();

	while(!anim.empty())
	{
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) * Scene::config->framesPerSecond;
		lastTime = nowTime;

		while(deltaTime >= 1.0 && !anim.empty())
		{
			Scene::camera->pos = anim.front();
			anim.erase(anim.begin());

			deltaTime--;
		}
	}


    Scene::camera->type=FREE;
    Scene::camera->inAnimation = false;
}
void cameraZoomOut()
{
    Scene::camera->type=ISOMETRIC;
    Scene::camera->inAnimation = true;
    std::vector<vec4> points;

    points.push_back(Scene::camera->pos);
    points.push_back(vec4(Scene::camera->pos.x,Scene::camera->pos.y,Scene::camera->pos.z + 3.0,1.0));
    points.push_back(glm::vec4(Scene::camera->lookat->x,Scene::camera->pos.y,(Scene::camera->lookat->z + 3.0),1.0));
    points.push_back(glm::vec4(Scene::camera->lookat->x,Scene::camera->lookat->y + 15.0,Scene::camera->lookat->z + 3.0,1.0));

    std::vector<glm::vec4> anim = bezierCurve(0.5,points);

	double nowTime = 0, deltaTime = 0, lastTime = glfwGetTime();

	while(!anim.empty())
	{
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) * Scene::config->framesPerSecond;
		lastTime = nowTime;

		while(deltaTime >= 1.0 && !anim.empty())
		{
			Scene::camera->pos = anim.front();
			anim.erase(anim.begin());

			deltaTime--;
		}
	}

    Scene::camera->inAnimation = false;
}

void Scene::keyEventHandler()
{
    if(!camera->inAnimation)
    {
        float t = config->t;

        if (g_WPressed)
        {

            if(camera->type == FREE)
            {
                glm::vec4 w = camera->view_vector;
                w.y = player->pos.y;
                w = normalize(w);

                player->pos += w*player->speed*t;
            }
            else
                player->pos.z -= player->speed*t;
        }
        if (g_SPressed)
        {
            if(camera->type == FREE)
            {
                glm::vec4 w = camera->view_vector;
                w.y = player->pos.y;
                w = normalize(w);

                player->pos -= w*player->speed*t;
            }
            else
                player->pos.z += player->speed*t;
        }
        if (g_APressed)
        {
            if(camera->type == FREE)
            {
                glm::vec4 w = camera->view_vector;
                w.y = player->pos.y;
                w = normalize(w);

                glm::vec4 u = crossproduct(camera->up_vector,w);

                player->pos += u*player->speed*t;
            }
            else
                player->pos.x -= player->speed*t;
        }
        if (g_DPressed)
        {
            if(camera->type == FREE)
            {
                glm::vec4 w = camera->view_vector;
                w.y = player->pos.y;
                w = normalize(w);

                glm::vec4 u = crossproduct(camera->up_vector,w);

                player->pos -= u*player->speed*t;
            }
            else
                player->pos.x += player->speed*t;
        }

        if(g_CPressed)
        {
            if(camera->type == ISOMETRIC)
            {
                std::thread(cameraZoomIn).detach();
                g_CameraPhi = 0.0f;
                g_CameraTheta = 0.0f;
            }
            else if(camera->type == FREE)
            {
                std::thread(cameraZoomOut).detach();
            }
            g_CPressed = false;
        }
    }
}

void Scene::renderInit()
{
    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles();

    //Carregamento das texturas
    LoadTextureImage("../../data/teste_3.png");
    LoadTextureImage("../../data/teste_5.png");

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}
void Scene::renderBaseline()
{
    // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
    // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
    // Vermelho, Verde, Azul, Alpha (valor de transparência).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
    //
    //           R     G     B     A
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
    // e também resetamos todos os pixels do Z-buffer (depth buffer).
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
    // os shaders de vértice e fragmentos).
    glUseProgram(program_id);
}

void Scene::renderOther()
{
    // Imprimimos na tela os ângulos de Euler que controlam a rotação do
    // terceiro cubo.
    TextRendering_ShowEulerAngles(window);

    // Imprimimos na informação sobre a matriz de projeção sendo utilizada.
    TextRendering_ShowProjection(window);

    // Imprimimos na tela informação sobre o número de quadros renderizados
    // por segundo (frames per second).
    TextRendering_ShowFramesPerSecond(window);

    TextRendering_ShowPlayerStats(window, Scene::player->hp, Scene::book->level);

    // O framebuffer onde OpenGL executa as operações de renderização não
    // é o mesmo que está sendo mostrado para o usuário, caso contrário
    // seria possível ver artefatos conhecidos como "screen tearing". A
    // chamada abaixo faz a troca dos buffers, mostrando para o usuário
    // tudo que foi renderizado pelas funções acima.
    // Veja o link: Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
    glfwSwapBuffers(window);

    // Verificamos com o sistema operacional se houve alguma interação do
    // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
    // definidas anteriormente usando glfwSet*Callback() serão chamadas
    // pela biblioteca GLFW.
    glfwPollEvents();
}

void Scene::loadModels()
{
    ObjModel model1("../../data/bunny.obj");
    ComputeNormals(&model1);
    BuildTrianglesAndAddToVirtualScene(&model1);

    ObjModel model2("../../data/plane.obj");
    ComputeNormals(&model2);
    BuildTrianglesAndAddToVirtualScene(&model2);

    ObjModel model3("../../data/sphere.obj");
    ComputeNormals(&model3);
    BuildTrianglesAndAddToVirtualScene(&model3);

    ObjModel model4("../../data/rock_cube.obj");
    ComputeNormals(&model4);
    BuildTrianglesAndAddToVirtualScene(&model4);
    //PrintObjModelInfo(&model4);

    ObjModel model5("../../data/tree_cone.obj");
    ComputeNormals(&model5);
    BuildTrianglesAndAddToVirtualScene(&model5);

    ObjModel model6("../../data/cube.obj");
    ComputeNormals(&model6);
    BuildTrianglesAndAddToVirtualScene(&model6);

    ObjModel model7("../../data/golem.obj");
    ComputeNormals(&model7);
    BuildTrianglesAndAddToVirtualScene(&model7);

}
void Scene::loadModels(std::vector<std::string> paths)
{
    for(const auto & path : paths)
    {
        ObjModel spheremodel(path.c_str());
        ComputeNormals(&spheremodel);
        BuildTrianglesAndAddToVirtualScene(&spheremodel);
    }
}

void Scene::clearObjects()
{
    for (Object * x : objects) {
        delete x;
    }
}

void drawHitbox()
{
    for(auto * object : Scene::objects)
    {
        std::string object_name;

        float sclX,sclY,sclZ;
        sclX = sclY = sclZ = 1;

        if(object->hitBoxType == BOX || object->hitBoxType == SPHERE)
        {
            object_name = "cube";
            sclX = object->hitbox.at(0).x - object->hitbox.at(1).x;
            sclY = object->hitbox.at(0).y - object->hitbox.at(1).y;
            sclZ = object->hitbox.at(0).z - object->hitbox.at(1).z;
        }
        else if(object->hitBoxType == SPHERE)
            object_name = "sphere";
        else
            continue;



        glm::mat4 modelMatrix = Matrix_Identity(); // Transformação identidade de modelagem

        modelMatrix = Matrix_Translate(object->pos.x,object->pos.y,object->pos.z)
                  * Matrix_Rotate_Z(object->rot.x)
                  * Matrix_Rotate_Y(object->rot.y)
                  * Matrix_Rotate_X(object->rot.z)
                  * Matrix_Scale(sclX * object->scl.x,sclY * object->scl.y,sclZ * object->scl.z);

        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(modelMatrix));

        vec3 Aux = object->Kd;

        if(object->contact || (object->i < 300))
        {
            object->Kd = vec3(1.0,0.0,0.0);
            object->i++;
        }

        float data_values[12] = {object->Kd.x,object->Kd.y,object->Kd.z,object->Ks.x,object->Ks.y,object->Ks.z,object->Ka.x,object->Ka.y,object->Ka.z,object->q,object->illumModel,object->objID};
        glUniform1fv(spectral_values_uniform, 11, data_values);

        DrawVirtualObject(object_name.c_str());

        object->Kd = Aux;
    }

}

#endif // SCENE_H
