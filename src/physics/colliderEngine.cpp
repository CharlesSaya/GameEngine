#include "headers/physics/colliderEngine.h"


ColliderEngine::ColliderEngine( float deltaTime ){
    this->deltaTime = deltaTime;
}

bool ColliderEngine::intersectAABB( AABB &box1, AABB &box2 ){
    return (  ( ( box1.getMin().x() < box2.getMax().x() ) && ( box1.getMax().x() > box2.getMin().x() ) )
           && ( ( box1.getMin().y() < box2.getMax().y() ) && ( box1.getMax().y() > box2.getMin().y() ) )
           && ( ( box1.getMin().z() > box2.getMax().z() ) && ( box1.getMax().z() < box2.getMin().z() ) ) );
}

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

float ColliderEngine::sweptAABB( QVector3D velocity, AABB& box1, AABB& box2, QVector3D& normal ){

    float dxEntry, dyEntry, dzEntry;
    float dxExit, dyExit, dzExit;

    QVector3D box1LastMinPosition = box1.getLastMin();
    QVector3D box1LastMaxPosition = box1.getLastMax();
    QVector3D box2MinPosition = box2.getMin();
    QVector3D box2MaxPosition = box2.getMax();
    qDebug() << "Box1"  << box1.getLastMin() << box1.getLastMax() << box1.getMin() << box1.getMax() ;
    qDebug() << "Box2"  << box2.getMin() << box2.getMax();

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
    qDebug() << xEntryTime << yEntryTime << zEntryTime;
    qDebug() << ( entryTime > exitTime );
    qDebug() <<  ( xEntryTime < 0.0 && yEntryTime < 0.0 && zEntryTime < 0.0 );
    qDebug() <<  (xEntryTime > 1.0 || yEntryTime > 1.0|| zEntryTime > 1.0 );
    if(  entryTime > exitTime
     || ( xEntryTime < 0.0 && yEntryTime < 0.0 && zEntryTime < 0.0 )
     ||   xEntryTime > 1.0 || yEntryTime > 1.0|| zEntryTime > 1.0 )

        return -1.0f;

    if( entryTime == xEntryTime ){
        if( velocity.x() > 0 )
            normal = QVector3D( -1., 0., 0. );
        else
            normal = QVector3D( 1., 0., 0. );
    }
    else if( entryTime == yEntryTime ){
        if( velocity.y() > 0 )
            normal = QVector3D( 0., -1., 0. );
        else
            normal = QVector3D( 0., 1., 0. );
    }
    else{
        if( velocity.z() > 0 )
            normal = QVector3D( 0., 0., 1. );
        else
            normal = QVector3D( 0., 0., -1. );
    }
    return entryTime;
}



const Terrain &ColliderEngine::getTerrain() const
{
    return terrain;
}

void ColliderEngine::setTerrain(const Terrain &newTerrain)
{
    terrain = newTerrain;
}

ColliderEngine::ColliderEngine(){

}
