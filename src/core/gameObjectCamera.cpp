#include "headers/core/gameObjectCamera.h"


GameObjectCamera::GameObjectCamera( std::string name, MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, GameObject * parent  ){

    this->name = name;
    this->moveComponent     = moveComponent;
    this->physicsComponent  = physicsComponent;
    this->colliderComponent = colliderComponent;
    this->parent = parent;
    this->transform = new Transform( this );

}

MoveComponent *GameObjectCamera::getMoveComponent()
{
    return moveComponent;
}

void GameObjectCamera::setMoveComponent(MoveComponent *newMoveComponent)
{
    moveComponent = newMoveComponent;
}

PhysicsComponent *GameObjectCamera::getPhysicsComponent()
{
    return physicsComponent;
}

void GameObjectCamera::setPhysicsComponent(PhysicsComponent *newPhysicsComponent)
{
    physicsComponent = newPhysicsComponent;
}

ColliderComponent *GameObjectCamera::getColliderComponent()
{
    return colliderComponent;
}

void GameObjectCamera::setColliderComponent(ColliderComponent *newColliderComponent)
{
    colliderComponent = newColliderComponent;
}
