#include "headers/core/gameObjectMesh.h"

/**
 * @brief Construction de mesh sans physique
 * @param name
 * @param meshRenderer
 * @param colliderComponent
 * @param isMovable
 * @param parent
 */
GameObjectMesh::GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent, bool isMovable, GameObject * parent  ){

    this->name = name;
    this->meshRenderer = meshRenderer;
    this->colliderComponent = colliderComponent;
    this->parent = parent;
    this->transform = new Transform( this );
    initSignalsSlots();
    this->isMovable = isMovable;

    if ( parent != nullptr )
        parent->addChild( this );
}

/**
 * @brief Constructeur de mesh ayant une physique
 * @param name
 * @param meshRenderer
 * @param colliderComponent
 * @param physicsComponent
 * @param isMovable
 * @param parent
 */

GameObjectMesh::GameObjectMesh( std::string name, MeshRenderer * meshRenderer, ColliderComponent * colliderComponent,PhysicsComponent *physicsComponent, bool isMovable, GameObject * parent  ){

    this->name = name;
    this->meshRenderer = meshRenderer;
    this->colliderComponent = colliderComponent;
    this->parent = parent;
    this->transform = new Transform( this );
    this->physicsComponent = physicsComponent;
    initSignalsSlots();
    this->isMovable = isMovable;

    if ( parent != nullptr )
        parent->addChild( this );
}

/**
 * @brief Initialise les connexions Signaux/Slots
 */

void GameObjectMesh::initSignalsSlots(){
    connect( transform, &Transform::transformed, this, &GameObjectMesh::hasTransformed );
    connect( this, &GameObjectMesh::updateAABB, meshRenderer, &MeshRenderer::updateAABB );
}

/**
 * @brief Slot activé lorsque l'objet a été déplacé
 */

void GameObjectMesh::hasTransformed(){
    QMatrix4x4 tr, rt, sc;
    tr.translate( this->transform->getPosition() );
    sc.scale( this->transform->getScale());
    emit updateAABB(  tr * sc );
}

// Getters & setters

void GameObjectMesh::setUseGravity(bool newUseGravity)
{
    useGravity = newUseGravity;
}

bool GameObjectMesh::getUseGravity() const
{
    return useGravity;
}

PhysicsComponent *GameObjectMesh::getPhysicsComponent() const
{
    return physicsComponent;
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
