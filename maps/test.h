#ifndef TEST_H
#define TEST_H

#include "enemy.h"
#include "Camera.h"
#include "scene.h"
#include "Timer.h"
#include <iostream>

void dynamic()
{
    double nowTime = 0, deltaTime = 0, lastTime = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / Scene::config->t;
        lastTime = nowTime;

        while(deltaTime >= 1.0)
        {
            for(auto & object : Scene::objects)
            {
                object->move();
            }
            for(auto & enemy : Scene::enemies)
            {
                enemy->pathfinding();

                //In here, we give a repulsion to enemies close to player
                vec4 distVec = Scene::player->pos - enemy->pos;
                if(norm(distVec) < 1.2)
                {
                    enemy->velocity = (4.0f * normalize(-distVec));
                }
                //In here, we give collision to enemies
                for(auto & enemy2 : Scene::enemies)
                {
                    if(enemy != enemy2)
                    {
                        vec4 distEnem = enemy2->pos - enemy->pos;
                        if(norm(distEnem) < 1.2)
                        {
                            enemy->pos += (3.0f * normalize(-distEnem) * Scene::config->t);
                        }
                    }
                }
            }

            deltaTime--;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Scene::test()
{
    double nowTime = 0, deltaTime = 0, lastTime = glfwGetTime();

    Scene::renderInit(); // Inicilização dos procedimentos de renderização
    Scene::loadModels(); // Construímos a representação de objetos geométricos através de malhas de triângulos

    float speed = 4.0;
    player = new Player("bunny",vec3(0.08,0.4,0.8),vec3(0.8,0.8,0.8),vec3(0.04,0.2,0.4),32.0,100,speed);
    Scene::player->pos.x = 6.0;

    //Scene::objects.push_back(player);

    camera = new Camera(vec4(0.0,0.0,0.0,1.0));
    camera->lookat = &Scene::player->pos;
    camera->farplane = -50.0f;

    //Scene::objects.push_back(camera);

    //Adds sphere in pos(-1.0,0.0,0.0);
    Dyn_Object sphere("sphere");
    sphere.pos.x = -1.0;

    //Gives it acceleration;
    sphere.acceleration = 0.1f * normalize(vec4(0.0,0.0,-1.0,0.0));
    Scene::objects.push_back(&sphere);

    //Ground
    Scene::objects.push_back(new Object("plane",vec4(0.0,-1.00,0.0,1.0),vec3(0.0,0.0,0.0),vec3(20.0,1.0,20.0),vec3(0.2,0.7,0.15),vec3(0.1,0.1,0.1),vec3(0.0,0.0,0.0),20.0));

    //Trees
    Scene::objects.push_back(new Object("tree_cone",vec4(1.0,-1.0,1.0,1.0),vec3(0.0,0.0,0.0),vec3(3.0,3.0,3.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(5.0,-1.0,8.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.0,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(-4.0,-1.0,7.0,1.0),vec3(0.0,0.0,0.0),vec3(3.5,3.5,3.5),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(-3.0,-1.0,-5.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.0,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(2.0,-1.0,-9.0,1.0),vec3(0.0,0.0,0.0),vec3(4.0,4.0,4.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(13.0,-1.0,-8.0,1.0),vec3(0.0,0.0,0.0),vec3(3.5,3.5,3.5),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(16.0,-1.0,-12.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.8,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(8.0,-1.0,-16.0,1.0),vec3(0.0,0.0,0.0),vec3(2.2,2.2,2.2),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));

    //Rocks
    Scene::objects.push_back(new Object("rock_cube",vec4(0.0,-1.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0),vec3(0.5,0.4,0.3),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),2.0));
    Scene::objects.push_back(new Object("rock_cube",vec4(4.0,-1.0,-18.0,1.0),vec3(0.0,0.0,0.0),vec3(1.0,2.0,2.4),vec3(0.5,0.4,0.3),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),2.0));

    //Enemies
    Scene::enemies.push_back(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));
    Scene::objects.push_back( Scene::enemies.back());

    Enemy::spawn();
    Enemy::spawn();
    Enemy::spawn();
    Enemy::spawn();

    //Prints distance between enemies and player (for those recently spawned, should be 15)
    for(auto & enemy : enemies)
    {
        float dist = norm(enemy->pos - player->pos);
        printf("%f\n", dist);
    }

    std::thread(dynamic).detach();

    // Ficamos em loop, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / Scene::config->t;
        lastTime = nowTime;

        // Aqui executamos as operações de renderização
        Scene::renderBaseline();

        camera->draw();
        //player->draw();
        if(camera->type != FREE)
            player->draw();


        while(deltaTime >= 1.0)
        {
            Scene::keyEventHandler();
            deltaTime--;
        }

        for(auto & object : objects)
        {
            object->draw();
        }

        Scene::renderOther();
    }

    Scene::clearObjects();
    delete player;
    delete camera;
}

#endif // TEST_H
