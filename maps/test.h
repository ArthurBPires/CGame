#ifndef TEST_H
#define TEST_H

#include "enemy.h"
#include "Camera.h"
#include "scene.h"
#include "Timer.h"
#include "collisions.h"
#include <iostream>
#include <typeinfo>

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
            for(auto * object : Scene::objects)
            {
                object->move();

                if(instanceof<Enemy>(object))
                        static_cast<Enemy *>(object)->pathfinding();

                for(auto * object2 : Scene::objects)
                {
                    if(object != object2)
                    {
                        if(object->isHit(object,object2))
                        {
                            if(instanceof<Player>(object) && instanceof<Enemy>(object2))
                            {
                                vec4 distVec = object->pos - object2->pos;
                                static_cast<Enemy *>(object2)->velocity = (4.0f * normalize(-distVec));
                            }
                            else if(!(instanceof<Enemy>(object) && instanceof<Player>(object2)))
                            {
                                vec4 distEnem = object2->pos - object->pos;
                                object->pos += (4.5f * normalize(-distEnem) * Scene::config->t);
                            }
                        }
                    }
                }
            }

            deltaTime--;
        }
    }
}

void Scene::test()
{
    double nowTime = 0, deltaTime = 0, lastTime = glfwGetTime();

    Scene::renderInit(); // Inicilização dos procedimentos de renderização
    Scene::loadModels(); // Construímos a representação de objetos geométricos através de malhas de triângulos

    float speed = 4.0;
    player = new Player("bunny",vec3(0.08,0.4,0.8),vec3(0.8,0.8,0.8),vec3(0.04,0.2,0.4),32.0,100,speed);
    player->pos.x = 6.0;
    player->hitBoxType = SPHERE;
    player->sphereRadius = 1.2f;

    Scene::objects.push_back(player);

    camera = new Camera(vec4(0.0,0.0,0.0,1.0));
    camera->lookat = &Scene::player->pos;
    camera->farplane = -50.0f;
    camera->hitBoxType = NONE;

    //Scene::objects.push_back(camera);

    //Adds sphere in pos(-1.0,0.0,0.0);
    Dyn_Object sphere("sphere");
    sphere.pos.x = -15.0;

    //Gives it acceleration;
    sphere.acceleration = 0.1f * normalize(vec4(0.0,0.0,-1.0,0.0));
    Scene::objects.push_back(&sphere);

    //Ground
    Object ground("plane",vec4(0.0,0.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(20.0,1.0,20.0),vec3(0.2,0.7,0.15),vec3(0.1,0.1,0.1),vec3(0.0,0.0,0.0),20.0);
    ground.hitBoxType = NONE;
    Scene::objects.push_back(&ground);

    //Trees
    Scene::objects.push_back(new Object("tree_cone",vec4(1.0,0.0,1.0,1.0),vec3(0.0,0.0,0.0),vec3(3.0,3.0,3.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(5.0,0.0,8.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.0,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(-4.0,0.0,7.0,1.0),vec3(0.0,0.0,0.0),vec3(3.5,3.5,3.5),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(-3.0,0.0,-5.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.0,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(2.0,0.0,-9.0,1.0),vec3(0.0,0.0,0.0),vec3(4.0,4.0,4.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(13.0,0.0,-8.0,1.0),vec3(0.0,0.0,0.0),vec3(3.5,3.5,3.5),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(16.0,0.0,-12.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.8,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(8.0,0.0,-16.0,1.0),vec3(0.0,0.0,0.0),vec3(2.2,2.2,2.2),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0));

    //Rocks
    Scene::objects.push_back(new Object("rock_cube",vec4(0.0,0.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0),vec3(0.5,0.4,0.3),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),2.0));
    Scene::objects.push_back(new Object("rock_cube",vec4(4.0,0.0,-18.0,1.0),vec3(0.0,0.0,0.0),vec3(1.0,2.0,2.4),vec3(0.5,0.4,0.3),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),2.0));

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

        for(auto * object : objects)
        {
            if(instanceof<Player>(object))
                object->draw();
            else if(camera->type != FREE)
                object->draw();
        }

        while(deltaTime >= 1.0)
        {
            Scene::keyEventHandler();
            deltaTime--;
        }

        if(g_HPressed)
            drawHitbox();

        Scene::renderOther();
    }

    Scene::clearObjects();
    delete player;
    delete camera;
}

#endif // TEST_H
