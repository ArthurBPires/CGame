#ifndef SCENE_H
#define SCENE_H

#include "Player.h"
#include "Camera.h"
#include "Timer.h"
#include <iostream>
#include <string>
#include <vector>

class Enemy;

struct Configuration
{
    float playerHPMod = 1.0;
    float playerSpeedMod = 1.0;

    float enemyHPMod = 1.0;
    float enemySpeedMod = 1.0;
    float enemyMaxSpeedMod = 1.0;
    double limitFPS = 1.0 / 60.0;
    float timeStep = limitFPS;


    Configuration(float playerHPMod = 1.0,float playerSpeedMod = 1.0,float enemyHPMod = 1.0,float enemySpeedMod = 1.0,float enemyMaxSpeedMod = 1.0):
        playerHPMod(playerHPMod),playerSpeedMod(playerSpeedMod),enemyHPMod(enemyHPMod),enemySpeedMod(enemySpeedMod),enemyMaxSpeedMod(enemyMaxSpeedMod){};
};

class Scene
{
    public:
        Scene();
        ~Scene();
        static Camera * camera;
        static Player * player;
        static std::vector<Enemy *> enemies;
        static std::vector<Object *> objects;

        static Configuration config;

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
Configuration Scene::config(1.0);
Camera * Scene::camera = NULL;
Player * Scene::player = NULL;
std::vector<Enemy *> Scene::enemies;
std::vector<Object *> Scene::objects;

Scene::Scene() {srand (static_cast <unsigned> (time(0)));};
Scene::~Scene() {};

void bezierCurve(int duration, int frames, std::vector<vec4> points, vec4 * position)
{
    int delay = std::round(duration / frames);

    float step = (float)(1.0f / frames);

    for(float t = 0.0f; t < 1.0f; t += step)
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
        } while(pointsAux.size() > 1);

        *position = pointsAux.front();

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void cameraZoomIn()
{
    Scene::camera->inAnimation = true;
    std::vector<vec4> points = std::vector<vec4>();
    points.push_back(Scene::camera->pos);
    points.push_back(glm::vec4(Scene::camera->pos.x,0.0,(Scene::camera->pos.z + 3.0),1.0));
    points.push_back(vec4(Scene::player->pos.x,0.0,Scene::player->pos.z,1.0));
    points.push_back(Scene::player->pos);
    bezierCurve(500,100,points,&(Scene::camera->pos));
    Scene::camera->type=FREE;
    Scene::camera->inAnimation = false;
}
void cameraZoomOut()
{
    Scene::camera->type=ISOMETRIC;
    Scene::camera->inAnimation = true;
    std::vector<vec4> points;

    points.push_back(Scene::camera->pos);
    points.push_back(vec4(Scene::camera->pos.x,0.0,Scene::camera->pos.z + 3.0,1.0));
    points.push_back(glm::vec4(Scene::camera->lookat->x,0.0,(Scene::camera->lookat->z + 3.0),1.0));
    points.push_back(glm::vec4(Scene::camera->lookat->x,Scene::camera->lookat->y + 15.0,Scene::camera->lookat->z + 3.0,1.0));
    bezierCurve(500,100,points,&(Scene::camera->pos));
    Scene::camera->inAnimation = false;
}

void Scene::keyEventHandler()
{
    if(!camera->inAnimation)
    {
        //auto newTime = Clock::now();
        //duration<float, std::milli> diff = newTime - player->timer;
        //float t = diff.count()/1000.0f;
        float t = config.timeStep;

        if (g_WPressed)
        {

            if(camera->type == FREE)
            {
                glm::vec4 w = camera->view_vector;
                w.y = Scene::player->pos.y;

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

        if(camera->type == LOOKAT)
        {
            camera->pos.x = camera->r*cos(g_CameraPhi)*sin(g_CameraTheta);
            camera->pos.y = camera->r*sin(g_CameraPhi);
            camera->pos.z = camera->r*cos(g_CameraPhi)*cos(g_CameraTheta);

            // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
            // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
            camera->view_vector = *(camera->lookat) - camera->pos; // Vetor "view", sentido para onde a câmera está virada
        }
        else if(camera->type == ISOMETRIC)
        {
            camera->pos.x = camera->lookat->x;
            camera->pos.y = camera->lookat->y + 15.0;
            camera->pos.z = camera->lookat->z + 3.0;

            // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
            // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
            camera->view_vector = *(camera->lookat) - camera->pos; // Vetor "view", sentido para onde a câmera está virada
        }
        else if(camera->type == FREE)
        {
            // Vetor "view", sentido para onde a câmera está virada
            camera->view_vector.z = -cos(g_CameraPhi)*cos(g_CameraTheta);
            camera->view_vector.y = -sin(g_CameraPhi);
            camera->view_vector.x = -cos(g_CameraPhi)*sin(g_CameraTheta);

            camera->pos.x = player->pos.x;
            camera->pos.y = player->pos.y;
            camera->pos.z = player->pos.z;
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
    else if(norm(camera->pos - player->pos) > 0.0005) //if camera in animation, look at player. (norm test to solve runtime error when *(camera->lookat) == camera->pos)
    {
        camera->view_vector = *(camera->lookat) - camera->pos;
    }
    /*
    if(camera->type != FREE)
        player->draw();


    camera->draw();
    */
}

void Scene::renderInit()
{
    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles();

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
    /*
    std::string path = "../../data";
    for (const auto & entry : fs::directory_iterator(path.c_str()))
    {
        ObjModel spheremodel(entry.path().string().c_str());
        ComputeNormals(&spheremodel);
        BuildTrianglesAndAddToVirtualScene(&spheremodel);
    }
    */
    ObjModel model1("../../data/bunny.obj");
    ComputeNormals(&model1);
    BuildTrianglesAndAddToVirtualScene(&model1);

    ObjModel model2("../../data/plane.obj");
    ComputeNormals(&model2);
    BuildTrianglesAndAddToVirtualScene(&model2);

    ObjModel model3("../../data/sphere.obj");
    ComputeNormals(&model3);
    BuildTrianglesAndAddToVirtualScene(&model3);
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

#endif // SCENE_H
