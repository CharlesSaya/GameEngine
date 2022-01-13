#ifndef COLLIDERENGINE_H
#define COLLIDERENGINE_H

#include "headers/game/node.h"

#include "headers/core/gameObject.h"
#include "headers/core/gameObjectMesh.h"
#include "headers/core/gameObjectPlayer.h"
#include "headers/core/colliderComponent.h"

struct CollisionData{
    QVector3D collisionPlanePoint;
    QVector3D collisionNormal;
    Node * node;
};

class ColliderEngine{

private:
    float EPSILON = 0.0005f;
    float deltaTime;
    Terrain terrain;
    std::vector<CollisionData> currentPlayerCollisions;
public:

    ColliderEngine();
    ColliderEngine( float deltaTime );

    bool intersectAABB( AABB &box1, AABB &box2 );
    AABB& buildBroadPhaseBox( AABB &broadPhaseBox, QVector3D velocity, AABB& box1  );
    float sweptAABB( QVector3D velocity, AABB& box1, AABB& box2, QVector3D& normal, QVector3D &planePoint );

    /**
     * Detect collision between a moving GameObject and a BVH
     */
    template< class Collidable>
    void detectCollision( Collidable * go, Node * node ){

        QVector3D objectVel =  go->getPhysicsComponent()->getVelocity();

        bool collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), node->nodeBoundingBox );

        if( node->children.empty() && ( node->gameObject->getName() != go->getName() )){
//            qDebug()<<(node->gameObject->getName().c_str() );
            if ( collision ){
                // Test if the collided object is a collectible
                if ( go->getName() == "Player" &&  node->gameObject->getIsCollectible()  ){
                    dynamic_cast<GameObjectPlayer*>(go)->getPlayerComponent()->addCollectible();

                    node->gameObject->destroy();
                }
                else{
                        QVector3D normal, planePoint;
                        float time = sweptAABB( objectVel, go->getMeshRenderer()->getMesh().getAABB(), node->nodeBoundingBox, normal, planePoint );
                        float distance = go->getWorldPosition().distanceToPlane( planePoint, normal );

                        if( time >= 0.){

                            if( go->getName() == "Player" ){

                                if(  resting( go, normal, distance ) && !isAlreadyCollidingWithPlayer( node->gameObject ) ){

                                    go->getPhysicsComponent()->setResting( true );
                                    currentPlayerCollisions.push_back( { planePoint, normal, node } );
                                }
                            }

                            computeCollision( time, distance, go, node->gameObject, normal );
                        }
                    }
                }

        }

        else if(collision){


            for( Node * childNode : node->children ){

                if( ( childNode->gameObject->getName() == go->getName() ) )
                    continue;

                bool collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), childNode->nodeBoundingBox );

                if( collision ){

                    if( go->getName() == "Player" )
                        qDebug() <<  childNode->gameObject->getName().c_str() << childNode->nodeBoundingBox.getMin() << childNode->nodeBoundingBox.getMax() ;

                    if ( childNode->gameObject->getName() == "Terrain"){

                        bool collision = intersectAABB( go->getMeshRenderer()->getMesh().getAABB(), dynamic_cast<GameObjectMesh*>(childNode->gameObject)->getMeshRenderer()->getMesh().getAABB() );
                        if( collision ){
                            float height;
                            QVector3D worldPos;
                            worldPos = go->getWorldPosition();
                            height = terrain.getHeightTerrain( worldPos );
                            if( worldPos.y() < height )
                                go->setHeight(height * terrain.getScale() );
                        }
                    }

                    detectCollision( go, childNode );

                }

            }

        }

    }
    /**
     * Detect if a go is already colliding with Player
     */
    bool isAlreadyCollidingWithPlayer( GameObject * go ){
        for( CollisionData data : this->currentPlayerCollisions )
            if( data.node->gameObject == go )
                return true;
        return false;
    }

    /**
     * Update the collision list of the goPlayer
     */
    void updatePlayerCurrentCollisionsList( GameObjectPlayer * goPlayer ){

        for( uint i = 0; i < currentPlayerCollisions.size(); i++ ){

            if( !intersectAABB( goPlayer->getMeshRenderer()->getMesh().getAABB(), dynamic_cast<GameObjectMesh*>(currentPlayerCollisions[i].node->gameObject)->getMeshRenderer()->getMesh().getAABB() ) ) {

                currentPlayerCollisions[i] = currentPlayerCollisions.back();
                currentPlayerCollisions.pop_back();
            }
        }
        if( currentPlayerCollisions.empty() )
            goPlayer->getPhysicsComponent()->setResting( false );
    }

    /**
     * return the distance between a position and the terrain
     */
    float distanceToTerrain( QVector3D  position, QVector3D pointOnPlane, QVector3D normal ){
        return QVector3D::dotProduct( position - pointOnPlane, normal );
    }

    /**
     * Compute collision between 2 gameObjects
     */
    template< class Collider, class Collided>
    void computeCollision( float time, float distance, Collider * collider, Collided * collided , QVector3D &normal ){

        QVector3D objectVel =  collider->getPhysicsComponent()->getVelocity();

        QVector3D vn = QVector3D::dotProduct( objectVel, normal ) * normal;
        QVector3D vt = objectVel - vn;

        QVector3D vnp = - collider->getPhysicsComponent()->getRestitution() * vn;
        QVector3D vtp = vt - std::min( collider->getPhysicsComponent()->getFriction() * vn.length(), vt.length() ) * vt.normalized();
        QVector3D vf = vnp + vtp;
        collider->move( -objectVel * ( 1.0f - time ) * deltaTime );
        collider->getPhysicsComponent()->setVelocity( vf );

//        if( collider->getName() == "Player" && collided->getIsMovable() )
//            dynamic_cast<GameObjectMesh*>(collided)->getPhysicsComponent()->setVelocity( objectVel );

    }

    /**
     * Return true if go is in resting phase
     */
    template<class Collidable>
    bool resting( Collidable * go, QVector3D& normal, float distance ){
        QVector3D objectVel =  go->getPhysicsComponent()->getVelocity();
        QVector3D vn = QVector3D::dotProduct( objectVel, normal ) * normal;
        QVector3D vt = objectVel - vn;
        if (objectVel.length() < 0.5 && distance < 0.01 && QVector3D::dotProduct(  go->getPhysicsComponent()->getAcceleration(), normal ) < 0.001 && vt.length() <  go->getPhysicsComponent()->getFriction() * vn.length()  ){
            return true;
        }
        return false;
    }

    Terrain &getTerrain();
    void setTerrain(const Terrain &newTerrain);

private:

};

#endif // COLLIDERENGINE_H
