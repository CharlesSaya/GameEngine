#include "headers/core/playerComponent.h"

PlayerComponent::PlayerComponent( Shader * shader, QObject * parent ){
    this->shader = shader;
    this->timer = new QTimer();
    connect( this->timer, &QTimer::timeout , this, &PlayerComponent::deactivateWheel );
    this->timer->start( wheelTimer );
}

PlayerComponent::PlayerComponent( QVector3D &playerPosition, QVector3D &playerDirection, Shader * shader, QObject * parent ){
    this->playerPosition  = playerPosition;
    this->playerDirection = playerDirection;
    this->timer = new QTimer();
    connect( this->timer, &QTimer::timeout , this, &PlayerComponent::deactivateWheel );
    this->timer->start( wheelTimer );
}

void PlayerComponent::update( QVector3D &playerPosition, QVector3D playerDirection ){
    this->playerPosition  = playerPosition;
    this->playerDirection = playerDirection;

}

Ray & PlayerComponent::castRay(){
    ray = Ray( this->playerPosition, this->playerDirection );
    return ray;
}

bool PlayerComponent::telekinesisActivated(){
    return this->rightMousePressed;
}

void PlayerComponent::telekinesis(  GameObject * player, GameObject * go ){
    if ( leftMousePressed ){

        linkedGO = go;

        if( go->getParent()->getName() != player->getName() )
            go->setParent( player );
    }
}

void PlayerComponent::pressedInput( QMouseEvent * key ){
    if( key->button() == Qt::RightButton ){
        rightMousePressed = true;
//        castRay();
    }

    if ( rightMousePressed && key->button() == Qt::LeftButton ){
        leftMousePressed = true;
    }
}

void PlayerComponent::releasedInput( QMouseEvent * key ){

    if( key->button() == Qt::RightButton ){
        rightMousePressed = false;
    }

    if ( key->button() == Qt::LeftButton ){
        leftMousePressed = false;
        if( linkedGO != nullptr ){
            linkedGO->setLastParent();
            linkedGO = nullptr;
        }
    }

}

void PlayerComponent::wheelScrolled( QWheelEvent * scroll ){

    timer->stop();
    if( leftMousePressed ){
        if( scroll->delta() > 0 ){
            wheelUp = true;
            wheelDown = false;
        }
        else{
            wheelDown = true;
            wheelUp = false;
        }
    }
    timer->start( wheelTimer );
}

void PlayerComponent::deactivateWheel(){
    wheelUp = false;
    wheelDown =false;
    timer->stop();
}

void PlayerComponent::drawRay( const QMatrix4x4& view, const QMatrix4x4& projection  ){
    if( rightMousePressed )

        ray.drawRay( shader, view, projection );

}

const Ray &PlayerComponent::getRay() const
{
    return ray;
}

void PlayerComponent::setRay(const Ray &newRay)
{
    ray = newRay;
}
