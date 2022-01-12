#ifndef GAMEOBJECTPLAYER_H
#define GAMEOBJECTPLAYER_H

#include <QSignalMapper>

#include "headers/core/gameObject.h"

#include "headers/core/meshrenderer.h"
#include "headers/core/moveComponent.h"
#include "headers/core/playerComponent.h"
#include "headers/core/physicsComponent.h"
#include "headers/core/colliderComponent.h"

class GameObjectPlayer : public GameObject{

    Q_OBJECT

signals:
    void updateAABB( const QMatrix4x4& model );
    void updatePlayerComponent( QVector3D &position, QVector3D direction );

public slots :
    void hasTransformed();

private:

    MeshRenderer * meshRenderer;
    MoveComponent * moveComponent;
    PlayerComponent * playerComponent;
    PhysicsComponent * physicsComponent;
    ColliderComponent * colliderComponent;

    QVector3D direction = QVector3D(0.0, 0.0, -1.0);

public :
    GameObjectPlayer();
    GameObjectPlayer( std::string name, MeshRenderer * meshRenderer, MoveComponent * moveComponent, PhysicsComponent * physicsComponent, ColliderComponent * colliderComponent, PlayerComponent * playerComponent, GameObject * parent  = 0 );

    void initSignalsSlots();

    MeshRenderer *getMeshRenderer() ;
    void setMeshRenderer(MeshRenderer *newMeshRenderer);
    MoveComponent *getMoveComponent() ;
    void setMoveComponent(MoveComponent *newMoveComponent);
    PhysicsComponent *getPhysicsComponent() ;
    void setPhysicsComponent(PhysicsComponent *newPhysicsComponent);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);

    PlayerComponent *getPlayerComponent() const;
    void setPlayerComponent(PlayerComponent *newPlayerComponent);

};

// MeshRenderer
// ColliderComponent
// PhysicsComponent
// MoveComponent


#endif // GAMEOBJECTPLAYER_H
