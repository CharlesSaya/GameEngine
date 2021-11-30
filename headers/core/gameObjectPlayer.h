#ifndef GAMEOBJECTPLAYER_H
#define GAMEOBJECTPLAYER_H

#include "headers/core/gameObject.h"

#include "headers/core/meshrenderer.h"
#include "headers/core/moveComponent.h"
#include "headers/core/physicsComponent.h"
#include "headers/core/colliderComponent.h"

class   GameObjectPlayer : public GameObject{


private:

    MeshRenderer * meshRenderer;
    MoveComponent * moveComponent;
    PhysicsComponent * physicsComponent;
    ColliderComponent * colliderComponent;

public :
    GameObjectPlayer();
    GameObjectPlayer( std::string name, MeshRenderer * meshRenderer, MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, GameObject * parent  = 0 );

    void initSignalsSlots();
    void input(QKeyEvent *key) override;
    void update(float step) override;
    void render(const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition) override;

    MeshRenderer *getMeshRenderer() ;
    void setMeshRenderer(MeshRenderer *newMeshRenderer);
    MoveComponent *getMoveComponent() ;
    void setMoveComponent(MoveComponent *newMoveComponent);
    PhysicsComponent *getPhysicsComponent() ;
    void setPhysicsComponent(PhysicsComponent *newPhysicsComponent);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);
};

// MeshRenderer
// ColliderComponent
// PhysicsComponent
// MoveComponent


#endif // GAMEOBJECTPLAYER_H
