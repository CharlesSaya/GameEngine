#include "headers/core/moveComponent.h"



MoveComponent::MoveComponent( Terrain & terrain, QObject * parent){

    this->terrain = terrain;

}

void MoveComponent::pressedInput(QKeyEvent * key){
    switch( key->key() ){

        case Qt::Key_Z:
            inputs.insert( 0 );
            move( inputs ) ;
            break;

        case Qt::Key_S:

            inputs.insert( 1 );
            move( inputs ) ;
            break;

        case Qt::Key_Q:
            inputs.insert( 2 );
            move( inputs ) ;
            break;

        case Qt::Key_D:
            inputs.insert( 3 );
            move( inputs ) ;
            break;

        case Qt::Key_Space:
            inputs.insert( 4 );
            move( inputs ) ;
            break;
    }
}

QQuaternion MoveComponent::getRotationY()
{
    return rotationY;
}

QQuaternion MoveComponent::getRotationX()
{
//    qDebug() << "rotationX" <<rotationX;
    return rotationX;
}


void MoveComponent::releasedInput(QKeyEvent * key){
    switch( key->key() ){

        case Qt::Key_Z :
            inputs.remove( 0 );
            stop( inputs );
            break;

        case Qt::Key_S :
            inputs.remove( 1 );
            stop( inputs );
            break;

        case Qt::Key_Q :
            inputs.remove( 2 );
            stop( inputs );
            break;

        case Qt::Key_D :
            inputs.remove( 3 );
            stop( inputs );
            break;

        case Qt::Key_Space :
            inputs.remove( 4 );
            stop( inputs );
            break;
    }
}

void MoveComponent::mouseMoveEvent(QVector2D pos, bool reset){
    float hAngle = 0., vAngle = 0.;
    float sensitivity = 0.05f;
    vAngle = mousePressPosition.y() -  pos.y() ;
    vAngle *= sensitivity * 3.;

    if( !reset ){
        hAngle = pos.x() -mousePressPosition.x() ;
        hAngle *= sensitivity *6;
    }

    mousePressPosition = pos;

     yaw = hAngle;
     pitch = vAngle;

    rotationAxis[0] -= pitch;
    rotationAxis[1] -= yaw ;
    if(rotationAxis[0]>60) rotationAxis[0] = 60.0f;
    if(rotationAxis[0]<-60) rotationAxis[0] = -60.0f;
    rotationX =  QQuaternion::fromAxisAndAngle( QVector3D(360.0f,0.0f,0.0f),  rotationAxis[0]);
    rotationY =  QQuaternion::fromAxisAndAngle( QVector3D(0.0f,360.0f,0.0f),  rotationAxis[1]);

    yaw = 0.0f;
    pitch = 0.0f;
}


void MoveComponent::update( float step ){

    float height;
    QVector3D worldPos;

//      worldPos = transform.getWorldPosition();
//      height = terrain.getHeight( worldPos );
//      transform.getPosition().setY( height );

}
