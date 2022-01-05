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

Ray & PlayerComponent::castRay(QVector3D target){

    ray = Ray( this->playerPosition, target);
    return ray;
}

bool PlayerComponent::telekinesisActivated(){
    return this->rightMousePressed;
}

void PlayerComponent::telekinesis(  GameObject * player, GameObject * go ){
    GameObjectMesh *child =  dynamic_cast<GameObjectMesh *>(go);
    if ( leftMousePressed ){
        if(!useTelekinesis && child->getIsMovable()){
            linkedGO = go;
            useTelekinesis = true;
            child->setUseGravity(false);
            if( go->getParent()->getName() != player->getName()){
                go->setParent( player );
            }
        }
    }
}

void PlayerComponent::attractAndPush(GameObject * go ){
        GameObject * child = go->getChildren()[go->getChildren().size()-1];

           if(wheelDown && (child->getWorldPosition() - go->getWorldPosition()).length() > minRange) {
               child->attract(0.50 );
           }
           else if(wheelUp&& (child->getWorldPosition() - go->getWorldPosition()).length() < maxRange){
               child->push(0.50f);
           }

    }

void PlayerComponent::pressedInput( QMouseEvent * key ){
    if( key->button() == Qt::RightButton ){
        rightMousePressed = true;
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
        useTelekinesis = false;
        if( linkedGO != nullptr ){
            linkedGO->setLastParent();
            dynamic_cast<GameObjectMesh *>(linkedGO)->setUseGravity(true);
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
