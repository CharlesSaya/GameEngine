#include "headers/core/gameObjectPlayer.h"



GameObjectPlayer::GameObjectPlayer( std::string name, MeshRenderer * meshRenderer, MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, GameObject * parent ){

    this->name = name;
    this->meshRenderer = meshRenderer;
    this->colliderComponent = colliderComponent;
    this->physicsComponent = physicsComponent;
    this->moveComponent = moveComponent;
    this->parent = parent;
    this->transform = new Transform( this );
    initSignalsSlots();
}

void GameObjectPlayer::initSignalsSlots(){
    connect( transform, &Transform::transformed, this, &GameObjectPlayer::hasTransformed );
    connect( this, &GameObjectPlayer::updateAABB, meshRenderer, &MeshRenderer::updateAABB );
    connect( moveComponent, &MoveComponent::move, physicsComponent, &PhysicsComponent::hasMoved );
    connect( moveComponent, &MoveComponent::stop, physicsComponent, &PhysicsComponent::hasStopped );
    connect( moveComponent, &MoveComponent::rotateX, physicsComponent, &PhysicsComponent::hasRotatedX );
    connect( moveComponent, &MoveComponent::rotateY, physicsComponent, &PhysicsComponent::hasRotatedY );

}

void GameObjectPlayer::hasTransformed(){
    emit( updateAABB( getModel() ));
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
