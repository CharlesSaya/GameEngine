#include "headers/core/gameObjectMesh.h"

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


void GameObjectMesh::initSignalsSlots(){
    connect( transform, &Transform::transformed, this, &GameObjectMesh::hasTransformed );
    connect( this, &GameObjectMesh::updateAABB, meshRenderer, &MeshRenderer::updateAABB );

}

void GameObjectMesh::hasTransformed(){
    QMatrix4x4 tr, rt, sc;
    tr.translate( this->transform->getPosition() );
    sc.scale( this->transform->getScale());
    emit updateAABB(  tr * sc );
}

void GameObjectMesh::addCollision( GameObject * go ){
    std::vector<GameObject *>::iterator itA;
    itA = find( this->currentCollisions.begin(), this->currentCollisions.end(), go);
    if ( itA ==this->currentCollisions.end()  )
        this->currentCollisions.push_back( go );
}

bool GameObjectMesh::isAlreadyCollidingWith( GameObject * go ){
    std::vector<GameObject *>::iterator itA;
    itA = find( this->currentCollisions.begin(), this->currentCollisions.end(), go);
    return itA !=this->currentCollisions.end();
}

void GameObjectMesh::updateRestingState(){
    if( currentCollisions.empty() )
        this->physicsComponent->setResting( false );
}

std::vector<GameObject *> &GameObjectMesh::getCurrentCollisions()
{
    return currentCollisions;
}


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
