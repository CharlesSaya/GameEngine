#include "headers/core/gameObjectPlayer.h"

/**
 * @brief GameObjectPlayer::GameObjectPlayer
 * @param name
 * @param meshRenderer
 * @param moveComponent
 * @param physicsComponent
 * @param colliderComponent
 * @param playerComponent
 * @param parent
 */

GameObjectPlayer::GameObjectPlayer( std::string name, MeshRenderer * meshRenderer, MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent,PlayerComponent * playerComponent,  GameObject * parent ){

    this->name = name;
    this->meshRenderer = meshRenderer;
    this->colliderComponent = colliderComponent;
    this->physicsComponent = physicsComponent;
    this->moveComponent = moveComponent;
    this->playerComponent = playerComponent;
    this->parent = parent;
    this->transform = new Transform( this );
    initSignalsSlots();

    if ( parent != nullptr )
        parent->addChild( this );
}

/**
 * @brief Initialise les connexions Signaux/Slots
 */

void GameObjectPlayer::initSignalsSlots(){

    connect( moveComponent, &MoveComponent::move, physicsComponent, &PhysicsComponent::hasMoved );
    connect( moveComponent, &MoveComponent::stop, physicsComponent, &PhysicsComponent::hasStopped );

    connect( transform, &Transform::transformed, this, &GameObjectPlayer::hasTransformed );

    connect( this, &GameObjectPlayer::updateAABB, meshRenderer, &MeshRenderer::updateAABB );
    connect( this, &GameObjectPlayer::updatePlayerComponent, playerComponent, &PlayerComponent::update );

    connect( moveComponent, &MoveComponent::moveChild, playerComponent, &PlayerComponent::hasMovedChild );
    connect( moveComponent, &MoveComponent::stopChild, playerComponent, &PlayerComponent::hasStoppedChild );
}

/**
 * @brief Slot activé lorsque l'objet a été déplacé
 */

void GameObjectPlayer::hasTransformed(){
    QMatrix4x4 tr, rt, sc;
    tr.translate( this->transform->getPosition() );
    sc.scale( this->transform->getScale());
    emit updateAABB(  tr * sc );
    emit updatePlayerComponent( this->getWorldPosition(), this->getTransform()->getRotation() * direction );
}

// Signals & Slots

PlayerComponent *GameObjectPlayer::getPlayerComponent() const
{
    return playerComponent;
}

void GameObjectPlayer::setPlayerComponent(PlayerComponent *newPlayerComponent)
{
    playerComponent = newPlayerComponent;
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
