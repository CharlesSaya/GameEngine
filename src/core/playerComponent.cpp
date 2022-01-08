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

void PlayerComponent::setPositionChild(GameObject * go,GameObject * child  ){

           if(wheelDown && (child->getWorldPosition() - go->getWorldPosition()).y() > rangeY[0]) {
               child->move(0.0f,-translationChildSpeed[1],0.0f );
           }
           else if(wheelUp&& (child->getWorldPosition() - go->getWorldPosition()).y() < rangeY[1]){
               child->move(0.0f,translationChildSpeed[1],0.0f);
           }

           moveChildZ();
           if( ( translationChildSpeed[2]>=0 && (child->getWorldPosition() - go->getWorldPosition()).length() > rangeZ[0])||
                (translationChildSpeed[2]<=0 && (child->getWorldPosition() - go->getWorldPosition()).length() < rangeZ[1])
            ){
               child->move(0.0f,0.0f,translationChildSpeed[2]*10 );
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

void PlayerComponent::moveChildZ(){
    for( uint i : inputsMoves ){
        switch( i ){
            case 0 :
                translationChildSpeed.setZ( speedZ);
                break;

            case 1 :
                translationChildSpeed.setZ( - speedZ);
                break;

        }
    }
}

int PlayerComponent::getCollectibleNumber() const
{
    return collectibleNumber;
}

void PlayerComponent::setCollectibleNumber(int newCollectibleNumber)
{
    collectibleNumber = newCollectibleNumber;
}

void PlayerComponent::addCollectible(){
    this->collectibleNumber++;
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



void PlayerComponent::hasMovedChild( QSet<uint> inputsMoves ){
    this->inputsMoves = inputsMoves;
}

void PlayerComponent::hasStoppedChild( QSet<uint> inputsMoves ){
    this->inputsMoves = inputsMoves;
    translationChildSpeed.setZ(0.0f);
}
