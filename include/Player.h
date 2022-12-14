#ifndef PLAYER_H
#define PLAYER_H

#include "Dyn_Object.h"


bool g_WPressed = false,g_SPressed = false,g_APressed = false,g_DPressed = false;
bool g_CPressed = false;
bool g_HPressed = false;

class Player : public Dyn_Object
{
    public:
        int hp = 100;
        float speed = 2.5;
        float timeInvulnerable = 0;

        Player(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q, int hp = 100, float speed = 0.0025) :
            Dyn_Object(model,pos,rot,scl,Kd,Ks,Ka,q), hp(hp), speed(speed) {};

        Player(std::string model,vec3 Kd, vec3 Ks, vec3 Ka, float q, int hp, float speed) :
            Dyn_Object(model,Kd,Ks,Ka,q), hp(hp), speed(speed) {};

        Player(std::string model) : Dyn_Object(model) {};

        Player(vec4 pos) : Dyn_Object(pos) {};

        Player(std::string model, vec4 velocity, vec4 acceleration) : Dyn_Object(model,velocity,acceleration) {};

        Player();

        ~Player();

    protected:

    private:
};

Player::~Player()
{

}

#endif // PLAYER_H


