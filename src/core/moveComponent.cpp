#include "headers/core/moveComponent.h"



MoveComponent::MoveComponent( Terrain & terrain ){

    this->terrain = terrain;

}

void MoveComponent::pressedInput(QKeyEvent * key){
    switch( key->key() ){

        case Qt::Key_Up:
            inputs.insert( 0 );
            move( inputs ) ;

            break;

        case Qt::Key_Down:

            inputs.insert( 1 );
            move( inputs ) ;

            break;

        case Qt::Key_Left:
            inputs.insert( 2 );
            move( inputs ) ;

            break;

        case Qt::Key_Right:
            inputs.insert( 3 );
            move( inputs ) ;

            break;

        case Qt::Key_Space:
            inputs.insert( 4 );
            move( inputs ) ;
            break;
    }
}



void MoveComponent::releasedInput(QKeyEvent * key){
    switch( key->key() ){

        case Qt::Key_Up :
            inputs.remove( 0 );
            stop( inputs );
            break;

        case Qt::Key_Down :
            inputs.remove( 1 );
            stop( inputs );
            break;

        case Qt::Key_Left :
            inputs.remove( 2 );
            stop( inputs );
            break;

        case Qt::Key_Right :
            inputs.remove( 3 );
            stop( inputs );
            break;

        case Qt::Key_Space :
            inputs.remove( 4 );
            stop( inputs );
            break;
    }
}

void MoveComponent::update( float step ){

    float height;
    QVector3D worldPos;

//      worldPos = transform.getWorldPosition();
//      height = terrain.getHeight( worldPos );
//      transform.getPosition().setY( height );

}
