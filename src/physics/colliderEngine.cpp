#include "headers/physics/colliderEngine.h"


ColliderEngine::ColliderEngine(){

}

bool ColliderEngine::intersectAABB( AABB &box1, AABB &box2 ){
    return (  ( ( box1.getMin().x() < box2.getMax().x() ) && ( box1.getMax().x() > box2.getMin().x() ) )
           || ( ( box1.getMin().y() < box2.getMax().y() ) && ( box1.getMax().y() > box2.getMin().y() ) )
              || ( ( box1.getMin().z() < box2.getMax().z() ) && ( box1.getMax().z() > box2.getMin().z() ) ) );
}

void ColliderEngine::detectCollision( GameObject * go, Node * node ){
    ColliderComponent * col =  static_cast<ColliderComponent *>( node->gameObject->getComponent<ColliderComponent>());

    if ( !intersectAABB( col->getMesh().getAABB(), *node->nodeBoundingBox ) )
        return;
    else
    {
        for ( Node * childNode : node->children ){
            if ( !childNode->children.empty() )
                detectCollision( go, childNode );
        }


    }

}

void ColliderEngine::computeCollision(){


}



