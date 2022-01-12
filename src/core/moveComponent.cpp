#include "headers/core/moveComponent.h"


MoveComponent::MoveComponent( Terrain & terrain, QObject * parent){

    this->terrain = terrain;

}

void MoveComponent::pressedInput(QKeyEvent * key){
    switch( key->key() ){

        case Qt::Key_Z:
            inputs.insert( 0 );
            emit move( inputs ) ;
            break;

        case Qt::Key_S:

            inputs.insert( 1 );
            emit move( inputs ) ;
            break;

        case Qt::Key_Q:
            inputs.insert( 2 );
            emit move( inputs ) ;
            break;

        case Qt::Key_D:
            inputs.insert( 3 );
            emit move( inputs ) ;
            break;

        case Qt::Key_Space:
            inputs.insert( 4 );
            emit move( inputs ) ;
            break;

        case Qt::Key_Control :
            inputs.insert( 5 );
            emit move( inputs );
            break;

        case Qt::Key_A :
            inputsChild.insert( 0 );
            emit moveChild (inputsChild );
            break;


        case Qt::Key_E :
            inputsChild.insert( 1 );
            emit moveChild(inputsChild );
            break;
    }
}

QQuaternion MoveComponent::getRotationY()
{
    return rotationY;
}

QQuaternion MoveComponent::getRotationX()
{
    return rotationX;
}


void MoveComponent::releasedInput(QKeyEvent * key){
    switch( key->key() ){

        case Qt::Key_Z :
            inputs.remove( 0 );
            emit stop( inputs );
            break;

        case Qt::Key_S :
            inputs.remove( 1 );
            emit stop( inputs );
            break;

        case Qt::Key_Q :
            inputs.remove( 2 );
            emit stop( inputs );
            break;

        case Qt::Key_D :
            inputs.remove( 3 );
            emit stop( inputs );
            break;

        case Qt::Key_Space :
            inputs.remove( 4 );
            emit stop( inputs );
            break;

        case Qt::Key_Control :
            inputs.remove( 5 );
            emit stop( inputs );
            break;

        case Qt::Key_A :
            inputsChild.remove( 0 );
            emit stopChild(inputsChild);
            break;


        case Qt::Key_E :
            inputsChild.remove( 1 );
            emit stopChild( inputsChild );
            break;
    }
}

void MoveComponent::mouseMoveEvent(QVector2D pos, bool resetX, bool resetY ){
    float hAngle = 0., vAngle = 0.;
    float sensitivity = 0.05f;

    if( !resetX ){
        hAngle = pos.x() -mousePressPosition.x();
        hAngle *= sensitivity *6;
    }

    if( !resetY ){
        vAngle = mousePressPosition.y() - pos.y();
        vAngle *= sensitivity * 5.;
    }

    mousePressPosition = pos;

    yaw = hAngle;
    pitch = vAngle;

    rotationAxis[0] -= pitch;
    rotationAxis[1] -= yaw ;
    if(rotationAxis[0]>90) rotationAxis[0] = 90.0f;
    if(rotationAxis[0]<-90) rotationAxis[0] = -90.0f;
    rotationX =  QQuaternion::fromAxisAndAngle( QVector3D(360.0f,0.0f,0.0f),  rotationAxis[0]);
    rotationY =  QQuaternion::fromAxisAndAngle( QVector3D(0.0f,360.0f,0.0f),  rotationAxis[1]);

    yaw = 0.0f;
    pitch = 0.0f;
}


void MoveComponent::update( float step ){

//    float height;
//    QVector3D worldPos;

//      worldPos = transform.getWorldPosition();
//      height = terrain.getHeight( worldPos );
//      transform.getPosition().setY( height );

}
