#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Object.h"

template< typename Type, typename DataType >
bool instanceof( const DataType * data ){
    return (typeid( *data ) == typeid( Type ));
}

bool isSphereHit(Object * obj1, Object * obj2, float radius)
{
    vec4 distVec = obj1->pos - obj2->pos;
    if(norm(distVec) < radius)
    {
        return true;
    }
    return false;
}

bool isInsideBox(glm::vec4 point, std::vector<glm::vec4> hitbox2)
{
    glm::vec4 pointMax = hitbox2.at(0);
    glm::vec4 pointMin = hitbox2.at(1);
    /*
    printf("Max: %f %f %f\n", pointMax.x, pointMax.y, pointMax.z);
    printf("Min: %f %f %f\n", pointMin.x, pointMin.y, pointMin.z);
    printf("point: %f %f %f\n", point.x, point.y, point.z);
    */
    if((point.x <= (pointMax.x)) && (point.y <= (pointMax.y)) && (point.z <= (pointMax.z)))
    {
        //printf("Almost\n");
        if((point.x >= (pointMin.x)) && (point.y >= (pointMin.y)) && (point.z >= (pointMin.z)))
        {
            //printf("hit\n");
            return true;
        }
    }
    return false;
}

bool isBoxHit(Object * obj1, Object * obj2)
{
    std::vector<glm::vec4> hitbox1 = obj1->hitbox;
    std::vector<glm::vec4> hitbox2 = obj2->hitbox;

    for(int i = 0; i < (int)hitbox1.size(); i++)
        hitbox1.at(i) = obj1->modelMatrix * hitbox1.at(i);
    for(int i = 0; i < (int)hitbox2.size(); i++)
        hitbox2.at(i) = obj2->modelMatrix * hitbox2.at(i);

    for(auto & point : hitbox1)
    {
        //printf("is %f %f %f inside\n", point.x,point.y,point.z);
        if(isInsideBox(point,hitbox2))
        {
            //printf("Inside\n");
            obj1->i = 0;
            obj2->i = 0;
            obj1->contact = true;
            obj2->contact = true;
            return true;
        }
    }
    for(auto & point : hitbox2)
    {
        if(isInsideBox(point,hitbox1))
        {
            //printf("Inside\n");
            obj1->i = 0;
            obj2->i = 0;
            obj1->contact = true;
            obj2->contact = true;
            return true;
        }
    }
    obj1->contact = false;
    obj2->contact = false;
    return false;
}

bool Object::isHit(Object * obj1, Object * obj2)
{
    bool result = false;

    if(hitBoxType == BOX)
    {
        result = isBoxHit(obj1,obj2);
    }
    else if(hitBoxType == SPHERE)
    {
        result = isSphereHit(obj1,obj2,sphereRadius);
    }
    else if(hitBoxType == BOX_POINT)
    {
        std::vector<glm::vec4> hitbox1 = obj1->hitbox;

        for(int i = 0; i < (int)hitbox1.size(); i++)
            hitbox1.at(i) = obj1->modelMatrix * hitbox1.at(i);
        result = isInsideBox(obj2->pos, hitbox1);
    }

    return result;
}

#endif
