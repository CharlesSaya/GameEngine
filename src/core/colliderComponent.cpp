#include "headers/core/colliderComponent.h"


ColliderComponent::ColliderComponent(){

}


void ColliderComponent::update( float step ){


}

void ColliderComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){

}

// Getters & Setters
bool ColliderComponent::getMoveable() const
{
    return moveable;
}

void ColliderComponent::setMoveable(bool newMoveable)
{
    moveable = newMoveable;
}
