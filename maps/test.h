#ifndef TEST_H
#define TEST_H

#include "enemy.h"
#include "scene.h"

void Scene::test()
{
    Scene::renderInit(); // Inicilização dos procedimentos de renderização
    Scene::loadModels(); // Construímos a representação de objetos geométricos através de malhas de triângulos

    float speed = 0.005;
    player = new Player("bunny",vec3(0.08,0.4,0.8),vec3(0.8,0.8,0.8),vec3(0.04,0.2,0.4),32.0,100,speed);
    Scene::player->pos.x = 6.0;

    Scene::objects.push_back(player);

    camera = new Camera(vec4(0.0,0.0,0.0,1.0));
    camera->lookat = &Scene::player->pos;

    Scene::objects.push_back(camera);

    //Adds sphere in pos(-1.0,0.0,0.0);
    Dyn_Object sphere("sphere");
    sphere.pos.x = -1.0;
    //Gives it acceleration;
     sphere.acceleration = 0.00001f * normalize(vec4(0.0,0.0,-1.0,0.0));
    Scene::objects.push_back(&sphere);

    Scene::objects.push_back(new Object("plane",vec4(0.0,-1.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(20.0,1.0,20.0),vec3(0.2,0.7,0.15),vec3(0.1,0.1,0.1),vec3(0.0,0.0,0.0),20.0));

    Scene::enemies.push_back(new Enemy("bunny",vec3(0.8,0.4,0.4),vec3(0.8,0.8,0.8),vec3(0.8,0.2,0.2),32.0,100,0.00008,0.0025));
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

    // Ficamos em loop, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização
        Scene::renderBaseline();

        for(auto & object : objects)
        {
            object->move();
            object->draw();
        }

        for(auto & enemy : enemies)
        {
            enemy->pathfinding();

            //In here, we give a repulsion to enemies close to player;
            vec4 distVec = player->pos - enemy->pos;
            if(norm(distVec) < 1.0)
            {
                enemy->velocity += (0.025f * normalize(-distVec));
            }
        }
        //Moves player based on user input
        player->userMove();

        Scene::renderOther();
    }
}

#endif // TEST_H
