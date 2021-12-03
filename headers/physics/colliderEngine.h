#ifndef COLLIDERENGINE_H
#define COLLIDERENGINE_H

#include "headers/game/node.h"

#include "headers/core/gameObject.h"
#include "headers/core/gameComponent.h"
#include "headers/core/colliderComponent.h"

class ColliderEngine{

public:
    ColliderEngine();
    bool intersectAABB( AABB &box1, AABB &box2 );

    template< class Collidable>
    void detectCollision( Collidable * go, Node * node ){
        bool collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), node->nodeBoundingBox );

        if( node->gameObject == go )
            return;

        if( node->children.empty() && collision && node->gameObject != go ){
            qDebug() << "Collision";
            computeCollision<Collidable>();
        }

        else if(collision){
            for( Node * childNode : node->children ){
                collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), childNode->nodeBoundingBox );
                if( collision )
                    detectCollision( go, childNode );

            }

        }

    }

    template< class Collidable>
    void computeCollision(){


    }



private:

};

#endif // COLLIDERENGINE_H
