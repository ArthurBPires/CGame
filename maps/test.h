#ifndef TEST_H
#define TEST_H

#include "enemy.h"
#include "Camera.h"
#include "scene.h"
#include "Timer.h"
#include <iostream>

void dynamic()
{
    if(glfwWindowShouldClose(window))
        exit(0);
    for(auto & object : Scene::objects)
    {
        object->move();
    }
    for(auto & enemy : Scene::enemies)
    {
        enemy->pathfinding();

        //In here, we give a repulsion to enemies close to player;
        vec4 distVec = Scene::player->pos - enemy->pos;
        if(norm(distVec) < 1.2)
        {
            enemy->velocity = (0.15f * normalize(-distVec));
        }

        for(auto & enemy2 : Scene::enemies)
        {
            if(enemy != enemy2)
            {
                vec4 distEnem = enemy2->pos - enemy->pos;
                if(norm(distEnem) < 1.0)
                {
                    enemy->pos += (0.05f * normalize(-distEnem));
                }
            }
        }
    }

    //Scene::player->userMove();
    Timer t1{10,dynamic};
}
/*
void cameraThread()
{
    Scene::camera->inAnimation = true;
    std::vector<vec4> points;
    points.push_back(Scene::camera->pos);
    points.push_back(glm::vec4(Scene::camera->pos.x,0.0,(Scene::camera->pos.z + 3.0),1.0));//(Scene::camera->pos.y - 7.5)
    points.push_back(vec4(Scene::player->pos.x,0.0,Scene::player->pos.z,1.0));
    points.push_back(Scene::player->pos);
    bezierCurve(2000,100,points,&(Scene::camera->pos));
    Scene::camera->type=FREE;
    Scene::camera->inAnimation = false;
}
*/
void playerMov()
{
    //Moves player based on user input
    if(glfwWindowShouldClose(window))
        exit(0);
    Scene::player->userMove();
    Scene::camera->move();
    Timer t2{10,playerMov};
}

void Scene::test()
{
    Scene::renderInit(); // Inicilização dos procedimentos de renderização
    Scene::loadModels(); // Construímos a representação de objetos geométricos através de malhas de triângulos

    float speed = 0.025;
    player = new Player("bunny",vec3(0.08,0.4,0.8),vec3(0.8,0.8,0.8),vec3(0.04,0.2,0.4),32.0,100,speed);
    Scene::player->pos.x = 6.0;

    //Scene::objects.push_back(player);

    camera = new Camera(vec4(0.0,0.0,0.0,1.0));
    camera->lookat = &Scene::player->pos;

    //Scene::objects.push_back(camera);

    //Adds sphere in pos(-1.0,0.0,0.0);
    Dyn_Object sphere("sphere");
    sphere.pos.x = -1.0;
    //Gives it acceleration;
     sphere.acceleration = 0.0001f * normalize(vec4(0.0,0.0,-1.0,0.0));
    Scene::objects.push_back(&sphere);

    Scene::objects.push_back(new Object("plane",vec4(0.0,-1.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(20.0,1.0,20.0),vec3(0.2,0.7,0.15),vec3(0.1,0.1,0.1),vec3(0.0,0.0,0.0),20.0));

    Scene::enemies.push_back(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,0.0025,0.025));
    Scene::objects.push_back( Scene::enemies.back());

    Enemy::spawn();
    Enemy::spawn();
    Enemy::spawn();

    //Prints distance between enemies and player (for those recently spawned, should be 15)
    for(auto & enemy : enemies)
    {
        float dist = norm(enemy->pos - player->pos);
        printf("%f\n", dist);
    }

    Timer t1{10,dynamic};
    //std::thread thread_obj(playerMov);
    //Timer t2{100,cameraZoomIn};
    //Timer t3{8000,cameraZoomIn};
    //std::vector<vec4> points{vec4(0.0,0.0,0.0,1.0),vec4(4.0,6.0,0.0,1.0),vec4(10.0,6.0,0.0,1.0),vec4(14.0,0.0,0.0,1.0)};
    //bezierCurve(2000,2,points,&camera->pos);

    // Ficamos em loop, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização
        Scene::renderBaseline();


        for(auto & object : objects)
        {
            //object->move();
            object->draw();
        }
        Scene::keyEventHandler();
        //player->draw();
        //camera->draw();
        //Moves player based on user input
        //player->userMove();
        //Scene::keyEventHandler();
        //camera->draw();

        Scene::renderOther();

        //Sleep(5);
    }
}

#endif // TEST_H
