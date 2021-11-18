#include "headers/core/moveComponent.h"



MoveComponent::MoveComponent( float deltaTime, Transform transform ){
    this->deltaTime = deltaTime;
    this->transform = transform;
}

void MoveComponent::input( ){
    transform.applyTranslation( QVector3D() );
}

void MoveComponent::update(){

}

void MoveComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){

}
