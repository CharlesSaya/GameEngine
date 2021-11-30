#include "headers/physics/colliderEngine.h"


ColliderEngine::ColliderEngine(){

}

bool ColliderEngine::intersectAABB( AABB &box1, AABB &box2 ){
    return (  ( ( box1.getMin().x() < box2.getMax().x() ) && ( box1.getMax().x() > box2.getMin().x() ) )
           || ( ( box1.getMin().y() < box2.getMax().y() ) && ( box1.getMax().y() > box2.getMin().y() ) )
              || ( ( box1.getMin().z() < box2.getMax().z() ) && ( box1.getMax().z() > box2.getMin().z() ) ) );
}



