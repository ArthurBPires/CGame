#ifndef OBJECT_H
#define OBJECT_H

#include "main.h"

using namespace glm;

class Object
{
    public:
        bool contact = false;
        bool shouldDraw = true;

        std::string model;
        glm::vec4 pos = vec4(0.0f,0.0f,0.0f,1.0f);
        glm::vec3 rot = vec3(0.0f,0.0f,0.0f);
        glm::vec3 scl = vec3(1.0f,1.0f,1.0f);

        // Parâmetros padrão que definem as propriedades espectrais da superfície
        glm::vec3 Kd = vec3(0.8,0.4,0.08); // Refletância difusa
        glm::vec3 Ks = vec3(0.0,0.0,0.0); // Refletância especular
        glm::vec3 Ka = vec3(0.4,0.2,0.04); // Refletância ambiente
        float q = 1.0; // Expoente especular para o modelo de iluminação de Phong
        float illumModel = 1.0; //1.0 = Phong; 2.0 = Blinn-Phong;
        float objID = 0.0; //0.0 = Objeto sem textura; 1.0 = Árvore;

        Object(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q, float illumModel, float objID) :
            model(model),pos(pos), rot(rot), scl(scl), Kd(Kd), Ks(Ks), Ka(Ka), q(q), illumModel(illumModel), objID(objID) {};
        Object(std::string model,vec4 pos, vec3 rot, vec3 scl, vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            model(model),pos(pos), rot(rot), scl(scl), Kd(Kd), Ks(Ks), Ka(Ka), q(q) {};
        Object(std::string model,vec3 Kd, vec3 Ks, vec3 Ka, float q) :
            model(model), Kd(Kd), Ks(Ks), Ka(Ka), q(q) {};
        Object(std::string model) : model(model) {};

        Object(vec4 pos) : pos(pos) {};

        virtual void draw();

        virtual void move() {};
        virtual ~Object();

    protected:

    private:
};

void Object::draw()
{
    //printf("objID: %f\nKd.x: %f\nKd.y: %f\nKd.z: %f\n", objID, Kd.x, Kd.y, Kd.z);
    glm::mat4 modelMatrix = Matrix_Identity(); // Transformação identidade de modelagem
    modelMatrix = Matrix_Translate(pos.x,pos.y,pos.z)
              * Matrix_Rotate_Z(rot.x)
              * Matrix_Rotate_Y(rot.y)
              * Matrix_Rotate_X(rot.z)
              * Matrix_Scale(scl.x,scl.y,scl.z);
    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(modelMatrix));

    float data_values[12] = {Kd.x,Kd.y,Kd.z,Ks.x,Ks.y,Ks.z,Ka.x,Ka.y,Ka.z,q,illumModel,objID};
    glUniform1fv(spectral_values_uniform, 11, data_values);

    if(model == "bunny"){
        glUniform1i(object_id_uniform, 1);
    }
    else if(model == "sphere"){
        glUniform1i(object_id_uniform, 2);
    }
    else{
        glUniform1i(object_id_uniform, 0);
    }

    DrawVirtualObject(model.c_str());
}

Object::~Object()
{
    //dtor
}


#endif // OBJECT_H
