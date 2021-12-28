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

public :
    GameObjectMesh();
    GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent, GameObject * parent  = 0  );

    void initSignalsSlots();

    MeshRenderer *getMeshRenderer() ;
    void setMeshRenderer(MeshRenderer *newMeshRenderer);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);
};
// MeshRenderer


#endif // GAMEOBJECTMESH_H
