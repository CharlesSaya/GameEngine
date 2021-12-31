#include "headers/core/gameObjectCamera.h"

GameObjectCamera::GameObjectCamera( std::string name, CameraComponent *cameraComponent ,MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, GameObject * parent  ){

    this->name = name;
    this->cameraComponent = cameraComponent;
    this->moveComponent     = moveComponent;
    this->physicsComponent  = physicsComponent;
    this->colliderComponent = colliderComponent;
    this->parent = parent;
    this->transform = new Transform( this );

    if ( parent != nullptr )
        parent->addChild( this );
}

GameObjectCamera::GameObjectCamera( std::string name, CameraComponent *cameraOrthoComponent  ){

    this->name = name;
    this->cameraComponent = cameraOrthoComponent;

//    this->parent = parent;
//    this->transform = new Transform( this );

//    if ( parent != nullptr )
//        parent->addChild( this );

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


CameraComponent *GameObjectCamera::getCameraComponent() const
{
    return cameraComponent;
}

void GameObjectCamera::setCameraComponent(CameraComponent *newCameraComponent)
{
    cameraComponent = newCameraComponent;
}


void GameObjectCamera::updateCameraPosition()
{

    this->cameraComponent->updatePosition(this->getModel());
}
