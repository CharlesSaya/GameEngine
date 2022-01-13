#include "headers/core/gameObjectCamera.h"

/**
 * @brief Constructeur de caméra perspective
 * @param name
 * @param cameraComponent
 * @param moveComponent
 * @param physicsComponent
 * @param colliderComponent
 * @param parent
 */

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

/**
 * @brief Constructeur de caméra orthogonale
 * @param name
 * @param cameraOrthoComponent
 */

GameObjectCamera::GameObjectCamera( std::string name, CameraComponent *cameraOrthoComponent  ){

    this->name = name;
    this->cameraComponent = cameraOrthoComponent;

}

/**
 * @brief Mets à jour la position de la caméra
 */

void GameObjectCamera::updateCameraPosition()
{
    this->cameraComponent->updatePosition(this->getModel());
}

//Getters & Setters

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

