#include "headers/physics/colliderEngine.h"

bool compareFloat(float a, float b)
{
    return fabs(a - b) <  0.0005;
}

ColliderEngine::ColliderEngine( float deltaTime ){
    this->deltaTime = deltaTime;
}

/**
 * detect if 2 AABB are intersecting
 */
bool ColliderEngine::intersectAABB( AABB &box1, AABB &box2 ){


    return (  ( ( box1.getMin().x() <= box2.getMax().x() ) && ( box1.getMax().x() >= box2.getMin().x() ) )
           && ( ( ( box1.getMin().y() ) -  EPSILON <=  (  box2.getMax().y() ) )  && ( box1.getMax().y()  -  EPSILON  >= box2.getMin().y() ))
           && ( ( box1.getMin().z() >= box2.getMax().z() ) && ( box1.getMax().z() <= box2.getMin().z() ) ) );
}

/**
 * build box for the broad phase
 */
AABB& ColliderEngine::buildBroadPhaseBox( AABB &broadPhaseBox, QVector3D velocity, AABB& box1 ){
    qDebug() << "LastBox"  << box1.getLastMin() << box1.getMax();
    qDebug() << "Box"  << box1.getMin() << box1.getMax();

    broadPhaseBox.getMin().setX( velocity.x()>0.0 ? box1.getLastMin().x() : box1.getLastMax().x() );
    broadPhaseBox.getMin().setY( velocity.y()>0.0 ? box1.getLastMin().y() : box1.getLastMax().y() );
    broadPhaseBox.getMin().setZ( velocity.z()>0.0 ? box1.getLastMin().z() : box1.getLastMax().z() );

    broadPhaseBox.getMax().setX( velocity.x()>0.0 ? box1.getMax().x() : box1.getMin().x() );
    broadPhaseBox.getMax().setY( velocity.y()>0.0 ? box1.getMax().y() : box1.getMin().y() );
    broadPhaseBox.getMax().setZ( velocity.z()>0.0 ? box1.getMax().z() : box1.getMin().z() );

    qDebug() << "BroadPhaseBox" << broadPhaseBox.getMin() << broadPhaseBox.getMax();

    return broadPhaseBox;
}

/**
 * Swept AABB
 */
float ColliderEngine::sweptAABB( QVector3D velocity, AABB& box1, AABB& box2, QVector3D& normal, QVector3D &planePoint ){

    float dxEntry, dyEntry, dzEntry;
    float dxExit, dyExit, dzExit;

    QVector3D box1LastMinPosition = box1.getLastMin();
    QVector3D box1LastMaxPosition = box1.getLastMax();
    QVector3D box2MinPosition = box2.getMin();
    QVector3D box2MaxPosition = box2.getMax();

    if( velocity.x() > 0. ){
        dxEntry = box2MinPosition.x() - box1LastMaxPosition.x();
        dxExit  = box2MaxPosition.x() - box1LastMinPosition.x();
    }
    else{
        dxEntry = box2MaxPosition.x() - box1LastMinPosition.x();
        dxExit  = box2MinPosition.x() - box1LastMaxPosition.x();
    }

    if( velocity.y()> 0. ){
        dyEntry = box2MinPosition.y() - box1LastMaxPosition.y();
        dyExit  = box2MaxPosition.y() - box1LastMinPosition.y();
    }
    else{
        dyEntry = box2MaxPosition.y() - box1LastMinPosition.y();
        dyExit  = box2MinPosition.y() - box1LastMaxPosition.y();
    }

    if( velocity.z()< 0. ){
        dzEntry = box2MinPosition.z() - box1LastMaxPosition.z();
        dzExit  = box2MaxPosition.z() - box1LastMinPosition.z();
    }
    else{
        dzEntry = box2MaxPosition.z() - box1LastMinPosition.z();
        dzExit  = box2MinPosition.z() - box1LastMaxPosition.z();
    }
    float xEntryTime, yEntryTime, zEntryTime;
    float xExitTime, yExitTime, zExitTime;

    if (velocity.x() == 0.){
        xEntryTime = -__FLT_MAX__;
        xExitTime  = __FLT_MAX__;
    }
    else{
        xEntryTime = dxEntry / velocity.x();
        xExitTime  = dxExit / velocity.x();
    }

    if (velocity.y() == 0.){
        yEntryTime = -__FLT_MAX__;
        yExitTime  = __FLT_MAX__;
    }
    else{
        yEntryTime = dyEntry / velocity.y();
        yExitTime  = dyExit / velocity.y();
    }

    if (velocity.z() == 0.){
        zEntryTime = -__FLT_MAX__;
        zExitTime  = __FLT_MAX__;
    }
    else{
        zEntryTime = dzEntry / velocity.z();
        zExitTime  = dzExit / velocity.z();
    }

    float entryTime = max(xEntryTime, max( yEntryTime, zEntryTime ) );
    float exitTime = min(xExitTime, min( yExitTime, zExitTime ) );

    if(  entryTime > exitTime
     || ( xEntryTime < 0.0 && yEntryTime < 0.0 && zEntryTime < 0.0 )
     ||   xEntryTime > 1.0 || yEntryTime > 1.0|| zEntryTime > 1.0 )

        return -1.0f;

    if( entryTime == xEntryTime ){
        if( velocity.x() > 0 ){
            normal = QVector3D( -1., 0., 0. );
            planePoint = box2MinPosition;
        }else{
            normal = QVector3D( 1., 0., 0. );
            planePoint = box2MaxPosition;

        }
    }
    else if( entryTime == yEntryTime ){
        if( velocity.y() > 0 ){
            normal = QVector3D( 0., -1., 0. );
            planePoint = box2MinPosition;

        }else{
            normal = QVector3D( 0., 1., 0. );
            planePoint = box2MaxPosition;

        }
    }
    else{
        if( velocity.z() > 0 ){
            normal = QVector3D( 0., 0., -1. );
            planePoint = box2MaxPosition;

        }else{
            normal = QVector3D( 0., 0., 1. );
            planePoint = box2MinPosition;

        }
    }
    return entryTime;
}



 Terrain &ColliderEngine::getTerrain()
{
    return terrain;
}

void ColliderEngine::setTerrain(const Terrain &newTerrain)
{
    terrain = newTerrain;
}

ColliderEngine::ColliderEngine(){

}
