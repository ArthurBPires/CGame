#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Player.h"
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>

namespace fs = std::experimental::filesystem;

class Enemy;

std::vector<Object *> Objects;
std::vector<Enemy *> Enemies;


class Scene
{
    public:
        Scene();
        ~Scene();
        static Player * player;
        void static renderInit();
        void static renderBaseline();
        void static renderOther();
        void static loadModels();
        void static loadModels(std::vector<std::string> paths);
        void static clearObjects();

    protected:

    private:
};
Player * Scene::player = new Player("bunny",vec3(0.08,0.4,0.8),vec3(0.8,0.8,0.8),vec3(0.04,0.2,0.4),32.0,100,0.0025);

void Scene::renderInit()
{
    // Carregamos os shaders de v�rtices e de fragmentos que ser�o utilizados
    // para renderiza��o. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles();

    // Inicializamos o c�digo para renderiza��o de texto.
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
    // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor �
    // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto �:
    // Vermelho, Verde, Azul, Alpha (valor de transpar�ncia).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Ilumina��o.
    //
    //           R     G     B     A
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
    // e tamb�m resetamos todos os pixels do Z-buffer (depth buffer).
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
    // os shaders de v�rtice e fragmentos).
    glUseProgram(program_id);
}

void Scene::renderOther()
{
    // Imprimimos na tela os �ngulos de Euler que controlam a rota��o do
    // terceiro cubo.
    TextRendering_ShowEulerAngles(window);

    // Imprimimos na informa��o sobre a matriz de proje��o sendo utilizada.
    TextRendering_ShowProjection(window);

    // Imprimimos na tela informa��o sobre o n�mero de quadros renderizados
    // por segundo (frames per second).
    TextRendering_ShowFramesPerSecond(window);

    // O framebuffer onde OpenGL executa as opera��es de renderiza��o n�o
    // � o mesmo que est� sendo mostrado para o usu�rio, caso contr�rio
    // seria poss�vel ver artefatos conhecidos como "screen tearing". A
    // chamada abaixo faz a troca dos buffers, mostrando para o usu�rio
    // tudo que foi renderizado pelas fun��es acima.
    // Veja o link: Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
    glfwSwapBuffers(window);

    // Verificamos com o sistema operacional se houve alguma intera��o do
    // usu�rio (teclado, mouse, ...). Caso positivo, as fun��es de callback
    // definidas anteriormente usando glfwSet*Callback() ser�o chamadas
    // pela biblioteca GLFW.
    glfwPollEvents();
}

void Scene::loadModels()
{
    std::string path = "../../data";
    for (const auto & entry : fs::directory_iterator(path.c_str()))
    {
        ObjModel spheremodel(entry.path().string().c_str());
        ComputeNormals(&spheremodel);
        BuildTrianglesAndAddToVirtualScene(&spheremodel);
    }
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
    for (Object * x : Objects) {
        delete x;
    }
}

#endif // SCENE_H
