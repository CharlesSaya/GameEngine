#ifndef GAMEOBJECTCAMERA_H
#define GAMEOBJECTCAMERA_H

#include "headers/core/gameObject.h"

#include "headers/core/moveComponent.h"
#include "headers/core/physicsComponent.h"
#include "headers/core/colliderComponent.h"
#include "headers/core/cameraComponent.h"

class GameObjectCamera : public GameObject{

private:

    CameraComponent * cameraComponent;
    MoveComponent * moveComponent;
    PhysicsComponent * physicsComponent;
    ColliderComponent * colliderComponent;

public :
    GameObjectCamera();
    GameObjectCamera( std::string name, CameraComponent *cameraComponent ,MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, GameObject * parent  = 0  );

    MoveComponent *getMoveComponent() ;
    void setMoveComponent(MoveComponent *newMoveComponent);
    PhysicsComponent *getPhysicsComponent() ;
    void setPhysicsComponent(PhysicsComponent *newPhysicsComponent);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);
    void updateCameraPosition();

    CameraComponent *getCameraComponent() const;
    void setCameraComponent(CameraComponent *newCameraComponent);
};
// ColliderComponent
// PhysicsComponent
// MoveComponent

#endif // GAMEOBJECTCAMERA_H
