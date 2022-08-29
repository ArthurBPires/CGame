
#ifndef DYN_OBJECT_H
#define DYN_OBJECT_H

#include "Object.h"
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::duration;

class Dyn_Object : public Object
{
    public:
        vec4 velocity = vec4(0.0,0.0,0.0,0.0);
        vec4 acceleration = vec4(0.0,0.0,0.0,0.0);

        Clock::time_point timer = Clock::now();

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
    //auto newTime = Clock::now();
    //duration<float, std::milli> diff = newTime - timer;
    //float t = diff.count()/1000.0f;
    float t = 1.0/60.0f;
    this->pos += this->velocity * t;
    this->velocity += this->acceleration * t;
    //timer = newTime;
}

Dyn_Object::~Dyn_Object()
{

}

#endif // DYN_OBJECT_H

