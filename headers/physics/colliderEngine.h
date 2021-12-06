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
            qDebug() << "Collision detected between " << go->getName().c_str() << node->gameObject->getName().c_str();
            QVector3D normal;
            float time = sweptAABB( objectVel, go->getMeshRenderer()->getMesh().getAABB(), node->nodeBoundingBox, normal );
            qDebug() << time ;
            if( time >= 0.)
                computeCollision( time, go, normal );
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

    template< class Collidable>
    void computeCollision( float time, Collidable * go, QVector3D &normal ){
        qDebug() <<" normal " << normal;
        QVector3D objectVel =  go->getPhysicsComponent()->getVelocity();
        QVector3D newVel = (1.-time) * objectVel;

        QVector3D vn = QVector3D::dotProduct( objectVel, normal ) * normal;
        QVector3D vt = objectVel - vn;

        QVector3D vnp = - go->getPhysicsComponent()->getRestitution() * vn;
        QVector3D vtp = vt - std::min( go->getPhysicsComponent()->getFriction() * vn.length(), vt.length() ) * vt.normalized();
        qDebug() << vnp;
        qDebug() << vtp;

        QVector3D vf = vnp + vtp;
        QVector3D response = (1.0 - time) * objectVel;
        go->move( -response * deltaTime  );
        go->getPhysicsComponent()->setVelocity( vf );

    }



private:

};

#endif // COLLIDERENGINE_H
