#include "headers/core/gameObjectMesh.h"

GameObjectMesh::GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent, GameObject * parent  ){

    this->name = name;
    this->meshRenderer = meshRenderer;
    this->colliderComponent = colliderComponent;
    this->parent = parent;
    this->transform = new Transform( this );
    initSignalsSlots();

    if ( parent != nullptr )
        parent->addChild( this );
}

void GameObjectMesh::initSignalsSlots(){
    connect( transform, &Transform::transformed, this, &GameObjectMesh::hasTransformed );
    connect( this, &GameObjectMesh::updateAABB, meshRenderer, &MeshRenderer::updateAABB );
}

void GameObjectMesh::hasTransformed(){
    emit updateAABB( getModel() );
}

MeshRenderer *GameObjectMesh::getMeshRenderer()
{
    return meshRenderer;
}

void GameObjectMesh::setMeshRenderer(MeshRenderer *newMeshRenderer)
{
    meshRenderer = newMeshRenderer;
}

ColliderComponent *GameObjectMesh::getColliderComponent()
{
    return colliderComponent;
}

void GameObjectMesh::setColliderComponent(ColliderComponent *newColliderComponent)
{
    colliderComponent = newColliderComponent;
}
