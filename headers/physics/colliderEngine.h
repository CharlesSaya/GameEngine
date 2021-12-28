#ifndef COLLIDERENGINE_H
#define COLLIDERENGINE_H

#include "headers/game/node.h"

#include "headers/core/gameObject.h"
#include "headers/core/gameComponent.h"
#include "headers/core/colliderComponent.h"

class ColliderEngine{

private:
    float deltaTime;

public:

    ColliderEngine();
    ColliderEngine( float deltaTime );

    bool intersectAABB( AABB &box1, AABB &box2 );
    AABB& buildBroadPhaseBox( AABB &broadPhaseBox, QVector3D velocity, AABB& box1  );
    float sweptAABB( QVector3D velocity, AABB& box1, AABB& box2, QVector3D& normal );

    template< class Collidable>
    void detectCollision( Collidable * go, Node * node ){

        QVector3D objectVel =  go->getPhysicsComponent()->getVelocity();

        AABB broadPhaseBox;
//        buildBroadPhaseBox( broadPhaseBox, objectVel, go->getMeshRenderer()->getMesh().getAABB());

        bool collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), node->nodeBoundingBox );

        if( node->children.empty() && collision ){
//            qDebug() << "Collision detected between " << go->getName().c_str() << node->gameObject->getName().c_str();
            QVector3D normal;
            float time = sweptAABB( objectVel, go->getMeshRenderer()->getMesh().getAABB(), node->nodeBoundingBox, normal );
            float distance = __FLT_MAX__;

            if( time >= 0.){
                if (node->gameObject->getName() == "Terrain")
                    distance = distanceToTerrain( go->getMeshRenderer()->getMesh().getAABB().getMin(), node->nodeBoundingBox.getMax(), normal  );
                resting( go, normal, distance );
                computeCollision( time, distance, go, normal );
            }
        }

        else if(collision){
            for( Node * childNode : node->children ){
                if( ( childNode->gameObject->getName() == go->getName() ) )
                    continue;

                bool collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), childNode->nodeBoundingBox );
                if( collision )
                    detectCollision( go, childNode );

            }

        }

    }

    float distanceToTerrain( QVector3D  position, QVector3D pointOnPlane, QVector3D normal ){
        return QVector3D::dotProduct( position - pointOnPlane, normal );
    }

    template< class Collidable>
    void computeCollision( float time, float distance, Collidable * go, QVector3D &normal ){

        QVector3D objectVel =  go->getPhysicsComponent()->getVelocity();

        QVector3D vn = QVector3D::dotProduct( objectVel, normal ) * normal;
        QVector3D vt = objectVel - vn;

        QVector3D vnp = - go->getPhysicsComponent()->getRestitution() * vn;
        QVector3D vtp = vt - std::min( go->getPhysicsComponent()->getFriction() * vn.length(), vt.length() ) * vt.normalized();

        QVector3D vf = vnp + vtp;

        go->move( -objectVel * deltaTime  );
        go->getPhysicsComponent()->setVelocity( vf );

    }

    template<class Collidable>
    void resting( Collidable * go, QVector3D& normal, float distance ){
        QVector3D objectVel =  go->getPhysicsComponent()->getVelocity();
        QVector3D vn = QVector3D::dotProduct( objectVel, normal ) * normal;
        QVector3D vt = objectVel - vn;
        if (objectVel.length() < 0.5 && distance < 0.01 && QVector3D::dotProduct(  go->getPhysicsComponent()->getAcceleration(), normal ) < 0.001 && vt.length() <  go->getPhysicsComponent()->getFriction() * vn.length()  ){
            go->getPhysicsComponent()->setResting( true );
        }


    }


private:

};

#endif // COLLIDERENGINE_H
