#ifndef TEST_H
#define TEST_H

#include "enemy.h"
#include "Camera.h"
#include "scene.h"
#include "Timer.h"
#include "collisions.h"
#include "weapons.h"
#include <iostream>
#include <typeinfo>

bool isDead(Object * enemy){
    return (enemy->shouldDraw == false);
}

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
            if(Scene::player->timeInvulnerable > 0){
                Scene::player->timeInvulnerable -= 1;
            }
            if(Scene::player->timeInvulnerable < 0){
                Scene::player->timeInvulnerable = 0;
            }

            for(auto & object : Scene::objects)
            {
                object->move();
            }
            for(auto * enemy : Scene::enemies)
            {
                if(enemy->shouldDraw){
                    if(enemy->timeInvulnerable > 0){
                        enemy->timeInvulnerable -= 1;
                    }
                    if(enemy->timeInvulnerable < 0){
                        enemy->timeInvulnerable = 0;
                    }

                    enemy->pathfinding();

                    //In here, we give a repulsion to enemies close to player

                    if(Scene::player->isHit(Scene::player,enemy))
                    {
                        vec4 distVec = Scene::player->pos - enemy->pos;
                        enemy->velocity = (4.0f * normalize(-distVec));
                        if(Scene::player->timeInvulnerable == 0){
                            Scene::player->hp -= 10;
                            Scene::player->timeInvulnerable = 10;
                        }
                    }
                    //Weapon collision detection
                    if(Scene::book->isHit(Scene::book,enemy))
                    {
                        vec4 distBook = Scene::book->pos - enemy->pos;
                        vec4 distVec = Scene::player->pos - enemy->pos;
                        enemy->velocity = (Scene::book->valKnbck*normalize(-distVec));
                        if(enemy->timeInvulnerable == 0){
                            enemy->hp -= Scene::book->contactDmg;
                            enemy->timeInvulnerable = 3;

                            if(enemy->hp <= 0){
                                enemy->hp = 0;
                                enemy->shouldDraw = false;

                                Scene::book->updateLevel(true);
                            }
                        }
                    }
                    //In here, we give collision to enemies
                    for(auto & enemy2 : Scene::enemies)
                    {
                        if(enemy != enemy2)
                        {
                            if(enemy->isHit(enemy,enemy2))
                            {
                                vec4 distEnem = enemy2->pos - enemy->pos;
                                enemy->pos += (3.0f * normalize(-distEnem) * Scene::config->t);
                            }
                        }
                    }
                }
            }

            auto it = remove_if(Scene::enemies.begin(), Scene::enemies.end(), isDead);
            Scene::enemies.erase(it, Scene::enemies.end());

            auto it2 = remove_if(Scene::objects.begin(), Scene::objects.end(), isDead);
            Scene::objects.erase(it2, Scene::objects.end());

            deltaTime--;
        }
    }
}

void Scene::test()
{
    double nowTime = 0, deltaTime = 0, lastTime = glfwGetTime(), timer = 0, timeToSpawn = 5 * Scene::config->framesPerSecond;
    int enemyAmount = 2;
    double hpMod = 1.0f;
    double sizeMod = 1.0f;

    Scene::renderInit(); // Inicilização dos procedimentos de renderização
    Scene::loadModels(); // Construímos a representação de objetos geométricos através de malhas de triângulos

    float speed = 4.0;
    player = new Player("bunny",vec3(0.08,0.4,0.8),vec3(0.8,0.8,0.8),vec3(0.04,0.2,0.4),32.0,100,speed);

    Scene::player->pos.x = 6.0;
    Scene::player->timeInvulnerable = 0;

    player->hitBoxType = SPHERE;
    player->sphereRadius = 1.2f;

    Scene::objects.push_back(player);

    camera = new Camera(vec4(0.0,0.0,0.0,1.0));
    camera->lookat = &Scene::player->pos;
    camera->nearplane = -0.5f;
    camera->farplane = -50.0f;
    camera->hitBoxType = NONE;

    //Scene::objects.push_back(camera);

    //Adds sphere in pos(-1.0,0.0,0.0);

    Dyn_Object sphere("sphere");
    sphere.pos.x = -15.0;

    //Gives it acceleration;
    //sphere.acceleration = 0.1f * normalize(vec4(0.0,0.0,-1.0,0.0));
    //Scene::objects.push_back(&sphere);

    //Spawns the book weapon
    book = new Book("sphere", vec4(Scene::player->pos.x, Scene::player->pos.y, Scene::player->pos.z - 4.0, 1.0), vec3(0.0,0.0,0.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 2.0);
    book->updateLevel(true);
    book->angleVal = 0;
    Scene::objects.push_back(book);

    //Ground
    Object ground("plane",vec4(0.0,0.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(20.0,1.0,20.0),vec3(0.2,0.7,0.15),vec3(0.1,0.1,0.1),vec3(0.0,0.0,0.0),20.0);
    ground.hitBoxType = PLANE;
    Scene::objects.push_back(&ground);

    //Carregamento da textura das árvores
    LoadTextureImage("../../data/teste_3.png");

    //Trees
    Scene::objects.push_back(new Object("tree_cone",vec4(1.0,0.0,1.0,1.0),vec3(0.0,0.0,0.0),vec3(3.0,3.0,3.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 2.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(5.0,0.0,8.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.0,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 2.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(-4.0,0.0,7.0,1.0),vec3(0.0,0.0,0.0),vec3(3.5,3.5,3.5),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 2.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(-3.0,0.0,-5.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.0,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 2.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(2.0,0.0,-9.0,1.0),vec3(0.0,0.0,0.0),vec3(4.0,4.0,4.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 1.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(13.0,0.0,-8.0,1.0),vec3(0.0,0.0,0.0),vec3(3.5,3.5,3.5),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 1.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(16.0,0.0,-12.0,1.0),vec3(0.0,0.0,0.0),vec3(2.0,2.8,2.0),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 1.0, 1.0));
    Scene::objects.push_back(new Object("tree_cone",vec4(8.0,0.0,-16.0,1.0),vec3(0.0,0.0,0.0),vec3(2.2,2.2,2.2),vec3(0.0,0.5,0.1),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),5.0, 1.0, 1.0));

    //Rocks
    Scene::objects.push_back(new Object("rock_cube",vec4(0.0,0.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0),vec3(0.5,0.4,0.3),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),2.0, 2.0, 0.0));
    Scene::objects.push_back(new Object("rock_cube",vec4(4.0,0.0,-18.0,1.0),vec3(0.0,0.0,0.0),vec3(1.0,2.0,2.4),vec3(0.5,0.4,0.3),vec3(0.0,0.0,0.0),vec3(0.1,0.2,0.1),2.0));


    //Enemies
    Scene::enemies.push_back(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));
    Scene::objects.push_back( Scene::enemies.back());

    Enemy::spawn(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));
    Enemy::spawn(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));

    Enemy::spawn(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));
    Enemy::spawn(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));

    Enemy::spawn(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));
    Enemy::spawn(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0));

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
        timer += (nowTime - lastTime) / Scene::config->t;
        lastTime = nowTime;

        // Aqui executamos as operações de renderização
        Scene::renderBaseline();

        camera->draw();

        book->action(deltaTime, Scene::player->pos);

        for(auto * object : objects)
        {
            if(!instanceof<Player>(object))
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

        while(timer >=  timeToSpawn)
        {
            for(int i=0; i< enemyAmount; i++)
            {
                Enemy * enemy1 = new Enemy("golem",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,1.5,1.0);
                enemy1->scl = vec3(0.2,0.2,0.25);
                enemy1->scl *= sizeMod;
                enemy1->hp *= hpMod;

                sizeMod *= 1.01;
                hpMod *= 1.00;

                Enemy::spawn(enemy1);
            }

            timer -= timeToSpawn;
        }

        if(Scene::player->hp <= 0){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    Scene::clearObjects();
    delete player;
    delete camera;
}

#endif // TEST_H
