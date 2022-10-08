#ifndef WEAPONS_H_INCLUDED
#define WEAPONS_H_INCLUDED

#define BOOK_START_LEVEL 0

#include "Object.h"
#include <math.h>

//Classe interface das armas
class Weapon : public Dyn_Object
{
    public:
        //Atributos presentes em todas armas
        int level = 0;
        float contactDmg;

        Weapon(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            Dyn_Object(model,pos,rot,scl,Kd,Ks,Ka,q) {};
};

//Arma do tipo orbital
class Book : public Weapon
{
    public:
        float angleSpeed;
        float angleVal;
        float valKnbck;

        Book(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            Weapon(model,pos,rot,scl,Kd,Ks,Ka,q) {};
        void action(float deltaTime, vec4 posPlayer);
        void updateLevel(bool incLevel);
};

//Funções de Book
void Book::action(float deltaTime, vec4 posPlayer){
    angleVal = angleVal + deltaTime*angleSpeed;

    rot += 0.02 * deltaTime;

    pos.y = posPlayer.y + 1.0;
    pos.x = posPlayer.x + cos(angleVal)*4.0;
    pos.z = posPlayer.z - sin(angleVal)*4.0;
};

void Book::updateLevel(bool incLevel){
    if(incLevel && level < 10){
        level += 1;
    }

    contactDmg = 10 + (level - 1)*2.0;
    angleSpeed = 0.005 + (level - 1)*0.004;
    valKnbck = 0.5 + (level - 1)*0.1;
    scl = vec3(1.0f,1.0f,1.0f) + ((level - 1) * 0.04f * vec3(1.0f,1.0f,1.0f));
};
//Fim das funções de Book

class Tornado : public Weapon{

    public:
        bool isOut = false;
        glm::vec4 trvlDir;
        float speed;
        float travelTime;
        float maxTravelTime;
        float loadTime;
        float fireRate;
        float valKnbck;

        Tornado(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            Weapon(model,pos,rot,scl,Kd,Ks,Ka,q) {};
        void action(float deltaTime, vec4 posPlayer);
        void updateLevel(bool incLevel);
};

void Tornado::action(float deltaTime, vec4 posPlayer){
    if(isOut){
        pos += speed*deltaTime*trvlDir;

        rot.y += 0.04 * deltaTime;

        travelTime += deltaTime;

        if(travelTime >= maxTravelTime){
            travelTime = 0;
            isOut = false;
            loadTime = 0;
        }
    }
    else{
        loadTime += deltaTime;

        if(loadTime >= fireRate){
            isOut = true;

            float randX = -1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0f)));
            float randZ = -1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0f)));
            trvlDir = glm::vec4(randX, 0.0f, randZ, 0.0);
            trvlDir = glm::normalize(trvlDir);

            pos = posPlayer;
        }
    }

};

void Tornado::updateLevel(bool incLevel){
    if(incLevel && level < 10){
        level += 1;
    }

    contactDmg = 14 + (level - 1)*2.5;
    speed = 0.05 + (level - 1)*0.015;
    fireRate = 300 - (level - 1)*30;
    maxTravelTime = 260 - (level - 1)*20;
    if(maxTravelTime < 80)
        maxTravelTime = 80;
    valKnbck = 0.65 + (level - 1)*0.15;
    scl = 1.2f*(vec3(1.0f,1.0f,1.0f) + ((level - 1) * 0.1f * vec3(1.0f,1.0f,1.0f)));
};

#endif // WEAPONS_H_INCLUDED
