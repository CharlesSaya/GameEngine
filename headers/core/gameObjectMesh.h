#ifndef GAMEOBJECTMESH_H
#define GAMEOBJECTMESH_H

#include "headers/core/gameObject.h"

#include "headers/core/meshrenderer.h"
#include "headers/core/colliderComponent.h"

class GameObjectMesh : public GameObject{

    Q_OBJECT

signals:
    void updateAABB( const QMatrix4x4& model );

public slots :
    void hasTransformed();

private:
    std::vector<GameObject *> currentCollisions;

    MeshRenderer * meshRenderer;
    ColliderComponent * colliderComponent;
    PhysicsComponent * physicsComponent;
    bool useGravity = true;

public :
    GameObjectMesh();
    GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent, bool isMovable = true , GameObject * parent  = 0);
    GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent,PhysicsComponent * physicsComponent, bool isMovable = true , GameObject * parent  = 0);

    void initSignalsSlots();

    MeshRenderer *getMeshRenderer() ;
    void setMeshRenderer(MeshRenderer *newMeshRenderer);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);

    PhysicsComponent *getPhysicsComponent() const;
    bool getUseGravity() const;
    void setUseGravity(bool newUseGravity);

    std::vector<GameObject *> &getCurrentCollisions();

    void addCollision( GameObject * go );
    bool isAlreadyCollidingWith( GameObject * go );
    void updateRestingState();

};
// MeshRenderer


#endif // GAMEOBJECTMESH_H
