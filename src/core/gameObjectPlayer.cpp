#include "headers/core/gameObjectPlayer.h"



GameObjectPlayer::GameObjectPlayer( std::string name, MeshRenderer * meshRenderer, MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, GameObject * parent ){

    this->name = name;
    this->meshRenderer = meshRenderer;
    this->colliderComponent = colliderComponent;
    this->physicsComponent = physicsComponent;
    this->moveComponent = moveComponent;
    this->parent = parent;
    this->transform = new Transform();
    initSignalsSlots();
}

void GameObjectPlayer::initSignalsSlots(){

    connect( transform, &Transform::transformed, meshRenderer, &MeshRenderer::updateBBox );
    connect( moveComponent, &MoveComponent::move, physicsComponent, &PhysicsComponent::hasMoved );
    connect( moveComponent, &MoveComponent::stop, physicsComponent, &PhysicsComponent::hasStopped );

}

void GameObjectPlayer::input( QKeyEvent * key ){

}

void GameObjectPlayer::update( float step ){


}

void GameObjectPlayer::render( const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition ){

}

MeshRenderer *GameObjectPlayer::getMeshRenderer()
{
    return meshRenderer;
}

void GameObjectPlayer::setMeshRenderer(MeshRenderer *newMeshRenderer)
{
    meshRenderer = newMeshRenderer;
}

MoveComponent *GameObjectPlayer::getMoveComponent()
{
    return moveComponent;
}

void GameObjectPlayer::setMoveComponent(MoveComponent *newMoveComponent)
{
    moveComponent = newMoveComponent;
}

PhysicsComponent *GameObjectPlayer::getPhysicsComponent()
{
    return physicsComponent;
}

void GameObjectPlayer::setPhysicsComponent(PhysicsComponent *newPhysicsComponent)
{
    physicsComponent = newPhysicsComponent;
}

ColliderComponent *GameObjectPlayer::getColliderComponent()
{
    return colliderComponent;
}

void GameObjectPlayer::setColliderComponent(ColliderComponent *newColliderComponent)
{
    colliderComponent = newColliderComponent;
}
