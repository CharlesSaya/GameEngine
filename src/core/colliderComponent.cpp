#include "headers/core/colliderComponent.h"


ColliderComponent::ColliderComponent( Mesh & mesh ):mesh(mesh){

}

void ColliderComponent::input( QKeyEvent * key  ){

}

void ColliderComponent::update( float step ){


}

void ColliderComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){

//    Shader * shader = this->mesh.getShader();
//    shader->useShaderProgram();

//    shader->setUniformValue( "model", model );
//    shader->setUniformValue( "view", view );
//    shader->setUniformValue( "projection", projection );
//    shader->setUniformValue( "cameraPosition", cameraPosition );

//    this->mesh.getAABB().drawAABB( this->mesh.getShader() );
}

// Getters & Setters

Mesh &ColliderComponent::getMesh()
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
