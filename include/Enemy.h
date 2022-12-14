#ifndef ENEMY_H
#define ENEMY_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "Scene.h"
#include <cstdlib>
#include <ctime>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif // M_PI

class Enemy : public Dyn_Object
{
    public:
        int hp = 100;
        float timeInvulnerable = 0;
        float speed = 1.5;
        float maxSpeed = 1.0;

        Enemy(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q, int hp, float speed=0.00025, float maxSpeed=0.0025) :
            Dyn_Object(model,pos,rot,scl,Kd,Ks,Ka,q), hp(hp), speed(speed), maxSpeed(maxSpeed) {};

        Enemy(std::string model,vec3 Kd, vec3 Ks, vec3 Ka, float q, int hp, float speed, float maxSpeed) :
            Dyn_Object(model,Kd,Ks,Ka,q), hp(hp), speed(speed), maxSpeed(maxSpeed) {};

        Enemy(std::string model) : Dyn_Object(model) {};

        Enemy(vec4 pos) : Dyn_Object(pos) {};

        Enemy(std::string model, vec4 velocity, vec4 acceleration) : Dyn_Object(model,velocity,acceleration) {};

        void pathfinding();
        static void spawn(Enemy * newEnemy, int dist);

        ~Enemy();

    protected:

    private:
};

void Enemy::spawn(Enemy * newEnemy,int dist = 15.0)
{
    //value between -pi and +pi
    float delta = (-M_PI) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(M_PI-(-M_PI))));

    newEnemy->pos.x = Scene::player->pos.x + (dist * cos(delta));
    newEnemy->pos.z = Scene::player->pos.z + (dist * sin(delta));

    Scene::objects.push_back(newEnemy);
    Scene::enemies.push_back(newEnemy);
}

void Enemy::pathfinding()
{
    if(Scene::player)
    {
        float x = 0,y = 0,z = 0;
        vec4 dir = Scene::player->pos - this->pos;
        dir = normalize(dir);
        if((abs(this->velocity.x) < abs(dir.x * this->maxSpeed)) || (std::signbit(velocity.x) != std::signbit(dir.x)) )
        {
            x = dir.x * this->speed;
        }
        if((abs(this->velocity.y) < abs(dir.y * this->maxSpeed)) || (std::signbit(velocity.y) != std::signbit(dir.y)) )
        {
            y = dir.y * this->speed;
        }
        if((abs(this->velocity.z) < abs(dir.z * this->maxSpeed)) || (std::signbit(velocity.z) != std::signbit(dir.z)) )
        {
            z = dir.z * this->speed;
        }
        float t = Scene::config->t;
        this->velocity += vec4(x,y,z,0.0)*t;
    }
}

Enemy::~Enemy()
{

}

#endif // ENEMY_H



