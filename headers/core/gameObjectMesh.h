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

    MeshRenderer * meshRenderer;
    ColliderComponent * colliderComponent;
    bool isMovable;

public :
    GameObjectMesh();
    GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent,bool isMovable , GameObject * parent  = 0);

    void initSignalsSlots();

    MeshRenderer *getMeshRenderer() ;
    void setMeshRenderer(MeshRenderer *newMeshRenderer);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);
    bool getIsMovable() const;
};
// MeshRenderer


#endif // GAMEOBJECTMESH_H
