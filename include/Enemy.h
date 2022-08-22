#ifndef ENEMY_H
#define ENEMY_H

#include "Scene.h"
#include <cmath>

class Enemy : public Dyn_Object
{
    public:
        int hp = 100;
        float speed = 0.00025;
        float maxSpeed = 0.25;

        Enemy(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q, int hp, float speed=0.00025, float maxSpeed=0.0025) :
            Dyn_Object(model,pos,rot,scl,Kd,Ks,Ka,q), hp(hp), speed(speed), maxSpeed(maxSpeed) {};

        Enemy(std::string model,vec3 Kd, vec3 Ks, vec3 Ka, float q, int hp, float speed, float maxSpeed) :
            Dyn_Object(model,Kd,Ks,Ka,q), hp(hp), speed(speed), maxSpeed(maxSpeed) {};

        Enemy(std::string model) : Dyn_Object(model) {};

        Enemy(vec4 pos) : Dyn_Object(pos) {};

        Enemy(std::string model, vec4 velocity, vec4 acceleration) : Dyn_Object(model,velocity,acceleration) {};

        void pathfinding();

        ~Enemy();

    protected:

    private:
};

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
        this->velocity += vec4(x,y,z,0.0);
        //printf("x: %f y: %f z: %f",velocity.x,velocity.y,velocity.z);
    }
}

Enemy::~Enemy()
{

}

#endif // ENEMY_H



