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

    pos.y = posPlayer.y;
    pos.x = posPlayer.x + cos(angleVal)*4.0;
    pos.z = posPlayer.z - sin(angleVal)*4.0;
};

void Book::updateLevel(bool incLevel){
    if(incLevel && level < 10){
        level += 1;
    }

    contactDmg = 10 + (level - 1)*2.0;
    angleSpeed = 0.005 + (level - 1)*0.001;
    valKnbck = 0.5 + (level - 1)*0.1;
};
//Fim das funções de Book

#endif // WEAPONS_H_INCLUDED
