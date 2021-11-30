#include "headers/core/moveComponent.h"



MoveComponent::MoveComponent( Terrain & terrain ){

    this->terrain = terrain;

}

void MoveComponent::pressedInput(QKeyEvent * key){
    switch( key->key() ){
        case Qt::Key_Up:
            emit move( forward );
            break;

        case Qt::Key_Down:
            emit move( backward );
            break;

        case Qt::Key_Left:
            emit move( left );
            break;

        case Qt::Key_Right:
            emit move( right );
            break;
        case Qt::Key_Shift:
            // TODO : Run !
            break;
    }
}

void MoveComponent::releasedInput(QKeyEvent * key){
    switch( key->key() ){
        case Qt::Key_Up : case Qt::Key_Down : case Qt::Key_Left : case Qt::Key_Right :
            emit stop();
            break;
    }
}

void MoveComponent::update( float step ){

    QVector3D worldPos;
    float height;
    //            worldPos = transform.getWorldPosition();
    //            height = terrain.getHeight( worldPos );
    //            transform.getPosition().setY( height );

}
