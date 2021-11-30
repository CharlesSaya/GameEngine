#ifndef GAMEOBJECTMESH_H
#define GAMEOBJECTMESH_H

#include "headers/core/gameObject.h"

#include "headers/core/meshrenderer.h"
#include "headers/core/colliderComponent.h"

class GameObjectMesh : public GameObject{

private:

    MeshRenderer * meshRenderer;
    ColliderComponent * colliderComponent;

public :
    GameObjectMesh();
    GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent, GameObject * parent  = 0  );


    void initSignalsSlots();
    void input(QKeyEvent *key) override;
    void update(float step) override;
    void render(const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition) override;

    MeshRenderer *getMeshRenderer() ;
    void setMeshRenderer(MeshRenderer *newMeshRenderer);
    ColliderComponent *getColliderComponent() ;
    void setColliderComponent(ColliderComponent *newColliderComponent);
};
// MeshRenderer


#endif // GAMEOBJECTMESH_H
