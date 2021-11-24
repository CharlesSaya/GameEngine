#include "headers/core/colliderComponent.h"


ColliderComponent::ColliderComponent( Mesh & mesh ):mesh(mesh){

}

void ColliderComponent::input( QKeyEvent * key  ){

}

void ColliderComponent::update( float step ){


}

void ColliderComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){
    // Maybe render AABB here ?

}

// Getters & Setters

Mesh &ColliderComponent::getMesh() const
{
    return mesh;
}

void ColliderComponent::setMesh(const Mesh &newMesh)
{
    mesh = newMesh;
}

boolean ColliderComponent::getMoveable() const
{
    return moveable;
}

void ColliderComponent::setMoveable(boolean newMoveable)
{
    moveable = newMoveable;
}
