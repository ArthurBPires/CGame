#ifndef OBJECT_H
#define OBJECT_H

#include "main.h"

using namespace glm;

enum hitBoxType {BOX,SPHERE,NONE};

class Object
{
    public:
        bool contact = false;
        unsigned long long i = 0;

        hitBoxType hitBoxType = BOX;
        float sphereRadius = 1.0f;

        std::string model;
        glm::mat4 modelMatrix = Matrix_Identity(); // Transformação identidade de modelagem
        std::vector<glm::vec4> hitbox;
        glm::vec4 pos = vec4(0.0f,0.0f,0.0f,1.0f);
        glm::vec3 rot = vec3(0.0f,0.0f,0.0f);
        glm::vec3 scl = vec3(1.0f,1.0f,1.0f);

        // Parâmetros padrão que definem as propriedades espectrais da superfície
        glm::vec3 Kd = vec3(0.8,0.4,0.08); // Refletância difusa
        glm::vec3 Ks = vec3(0.0,0.0,0.0); // Refletância especular
        glm::vec3 Ka = vec3(0.4,0.2,0.04); // Refletância ambiente
        float q = 1.0; // Expoente especular para o modelo de iluminação de Phong

        Object(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            model(model),pos(pos), rot(rot), scl(scl), Kd(Kd), Ks(Ks), Ka(Ka), q(q) {hitbox = g_VirtualScene[model.c_str()].hitbox;};
        Object(std::string model,vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            model(model), Kd(Kd), Ks(Ks), Ka(Ka), q(q) {hitbox = g_VirtualScene[model.c_str()].hitbox;};
        Object(std::string model) : model(model) {hitbox = g_VirtualScene[model.c_str()].hitbox;};

        Object(vec4 pos) : pos(pos) {};

        virtual void draw();

        bool isHit(Object * obj1, Object * obj2);

        virtual void move() {};
        virtual ~Object();

    protected:

    private:
};

void Object::draw()
{
    modelMatrix = Matrix_Translate(pos.x,pos.y,pos.z)
              * Matrix_Rotate_Z(rot.x)
              * Matrix_Rotate_Y(rot.y)
              * Matrix_Rotate_X(rot.z)
              * Matrix_Scale(scl.x,scl.y,scl.z);
    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(modelMatrix));

    float data_values[10] = {Kd.x,Kd.y,Kd.z,Ks.x,Ks.y,Ks.z,Ka.x,Ka.y,Ka.z,q};
    glUniform1fv(spectral_values_uniform, 10, data_values);
    DrawVirtualObject(model.c_str());

    //float sclX = norm(hitbox.at(5)-hitbox.at(1));
    //float sclY = norm(hitbox.at(7)-hitbox.at(1));
    //float sclZ = norm(hitbox.at(6)-hitbox.at(1));

    /*
    float sclX = hitbox.at(0).x - hitbox.at(1).x;
    float sclY = hitbox.at(0).y - hitbox.at(1).y;
    float sclZ = hitbox.at(0).z - hitbox.at(1).z;

    glm::mat4 modelMatrix = Matrix_Identity(); // Transformação identidade de modelagem
    modelMatrix = Matrix_Translate(pos.x,pos.y,pos.z)
              * Matrix_Rotate_Z(rot.x)
              * Matrix_Rotate_Y(rot.y)
              * Matrix_Rotate_X(rot.z)
              * Matrix_Scale(sclX,sclY,sclZ);
    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(modelMatrix));

    vec3 Aux = Kd;

    if(contact || (i < 500))
    {
        Kd = vec3(1.0,0.0,0.0);
        i++;
    }

    float data_values[10] = {Kd.x,Kd.y,Kd.z,Ks.x,Ks.y,Ks.z,Ka.x,Ka.y,Ka.z,q};
    glUniform1fv(spectral_values_uniform, 10, data_values);
    DrawVirtualObject("cube");

    Kd = Aux;
    */
}

Object::~Object()
{
    //dtor
}


#endif // OBJECT_H
