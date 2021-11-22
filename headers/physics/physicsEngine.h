#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "headers/render/AABB.h"

class PhysicsEngine
{
public:
    PhysicsEngine( float deltaTime );
    void computeForces();
    bool intersectAABB( AABB &box1, AABB &box2 );
private:

    float deltaTime;

    float gravity  = -9.81;
    float damp     = 1.0;
    float wind     = 1.0;
    float friction = 1.0;

};

#endif // PHYSICSENGINE_H
