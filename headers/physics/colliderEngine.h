#ifndef COLLIDERENGINE_H
#define COLLIDERENGINE_H

#include "headers/game/sceneGraph.h"
#include "headers/core/gameObject.h"
#include "headers/core/gameComponent.h"
#include "headers/core/colliderComponent.h"

class ColliderEngine{

public:
    ColliderEngine();
    void computeCollision();
    void detectCollision( GameObject * go, Node * node );
    bool intersectAABB( AABB &box1, AABB &box2 );


private:

};

#endif // COLLIDERENGINE_H
