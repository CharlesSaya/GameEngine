#include "headers/core/moveComponent.h"



MoveComponent::MoveComponent( float deltaTime, Terrain & terrain ){

    this->deltaTime      = deltaTime;
    this->terrain          = terrain;

}

void MoveComponent::input( QKeyEvent * key ){

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

void MoveComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){

}
