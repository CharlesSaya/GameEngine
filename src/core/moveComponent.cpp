#include "headers/core/moveComponent.h"



MoveComponent::MoveComponent( float deltaTime, Transform &transform, Terrain & terrain  ) : transform( transform ){

    this->deltaTime = deltaTime;
    this->terrain   = terrain;

}

bool MoveComponent::eventFilter( QObject * obj,  QEvent * event ){
    qDebug() << "feeee";
    if( event->type() == QEvent::KeyPress ){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        input(keyEvent);
        return true;

    }
    return false;
}

void MoveComponent::input( QKeyEvent * key ){

    QVector3D worldPos;
    float height;

    switch( key->key() ){
        case Qt::Key_Up:

            transform.applyTranslation( QVector3D( 0.,0.,-1. ) * movementSpeed );
            worldPos = transform.getWorldPosition();
            qDebug() << worldPos ;
            height = terrain.getHeight( worldPos );
            transform.getPosition().setY( height );
            break;

        case Qt::Key_Down:
            transform.applyTranslation( QVector3D( 0.,0.,1. ) * movementSpeed );
            worldPos = transform.getWorldPosition();
            height = terrain.getHeight( worldPos );
            transform.getPosition().setY( height );
            break;

        case Qt::Key_Left:

            transform.applyTranslation( QVector3D( -1.,0.,0. ) * movementSpeed );
            worldPos = transform.getWorldPosition();
            height = terrain.getHeight( worldPos );
            transform.getPosition().setY( height );
            break;

        case Qt::Key_Right:

            transform.applyTranslation( QVector3D( 1.,0.,0. ) * movementSpeed );
            worldPos = transform.getWorldPosition();
            height = terrain.getHeight( worldPos );
            transform.getPosition().setY( height );
            break;
    }

}

void MoveComponent::update(){

}

void MoveComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){

}
