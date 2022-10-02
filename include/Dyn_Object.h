
#ifndef DYN_OBJECT_H
#define DYN_OBJECT_H

#include "Object.h"
#include "Configuration.h"
#include <chrono>

class Dyn_Object : public Object
{
    public:
        vec4 velocity = vec4(0.0,0.0,0.0,0.0);
        vec4 acceleration = vec4(0.0,0.0,0.0,0.0);

        Dyn_Object(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q, float illumModel, float objID) :
            Object(model,pos,rot,scl,Kd,Ks,Ka,q,illumModel,objID) {};

        Dyn_Object(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            Object(model,pos,rot,scl,Kd,Ks,Ka,q) {};

        Dyn_Object(std::string model,vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            Object(model,Kd,Ks,Ka,q) {};

        Dyn_Object(std::string model) : Object(model) {};

        Dyn_Object(vec4 pos) : Object(pos) {};

        Dyn_Object(std::string model, vec4 velocity, vec4 acceleration) : Object(model),velocity(velocity), acceleration(acceleration){};

        void move();

        ~Dyn_Object();

    protected:

    private:
};
//When implemented, should be divided by TIME_STEP
void Dyn_Object::move()
{
    float t = g_config.t;
    this->pos += this->velocity * t;
    this->velocity += this->acceleration * t;
}

Dyn_Object::~Dyn_Object()
{

}

#endif // DYN_OBJECT_H

