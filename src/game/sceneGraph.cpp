#include "headers/game/sceneGraph.h"

const std::vector<GameObjectMesh *> &SceneGraph::getGoMeshes() const
{
    return goMeshes;
}

GameObjectPlayer *SceneGraph::getGoPlayer() const
{
    return goPlayer;
}

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( std::vector<GameObject *>& goList,
                        std::vector<GameObjectMesh *> &goMeshes,
                        GameObjectPlayer * &goPlayer,
                        GameObjectCamera * &goCamera,
                        PhysicsEngine& physicsEngine,
                        ColliderEngine & colliderEngine,
                        Terrain & terrain){

    this->goList = goList;
    this->goMeshes = goMeshes;
    this->goPlayer = goPlayer;
    this->goCamera = goCamera;
    this->terrain = terrain;
    this->root = new Node();
    for( GameObject * go : goList){
        this->root->children.push_back( buildGraphScene( go ) );
    }

    // set root AABB
    for( Node * childNode : this->root->children ){
        this->root->nodeBoundingBox.resizeAABB( childNode->nodeBoundingBox );
    }

    this->physicsEngine = physicsEngine;
    this->colliderEngine = colliderEngine;

}

Node * SceneGraph::buildGraphScene( GameObject * go ){
    Node * node = new Node();
    node->gameObject  = go;

    std::vector<GameObjectMesh *>::iterator itA;

    AABB bbox;
    itA = find( goMeshes.begin(), goMeshes.end(), go);

    if ( itA != goMeshes.end()  )
        bbox = (*itA)->getMeshRenderer()->getMesh().getAABB();


    else bbox = goPlayer->getMeshRenderer()->getMesh().getAABB();

    if( go->getChildren().empty() ){

        node->nodeBoundingBox.resizeAABB( bbox );
        return node;

    }else{
        node->nodeBoundingBox.resizeAABB( bbox );

        for( GameObject * child : go->getChildren() ){
            node->children.push_back( buildGraphScene( child ) );
            node->nodeBoundingBox.resizeAABB( (*node).children.back()->nodeBoundingBox );
        }
    }
    return node;
}

void SceneGraph::input(QKeyEvent *key){
}

void SceneGraph::update( float fixedStep ){
    // update player physics
      this->updatePhysics( goPlayer, fixedStep );
      goPlayer->rotate(goPlayer->getMoveComponent()->getRotationY());
      qDebug()<< goPlayer->getWorldPosition().x() << ',' << goPlayer->getWorldPosition().z();

      // update main camera position
      float angleX = goCamera->getMoveComponent()->getRotationX().toEulerAngles()[0];

      QQuaternion parentRotation = dynamic_cast<GameObjectPlayer*>(goCamera->getParent())->getMoveComponent()->getRotationY();
      float angleY = parentRotation.toEulerAngles()[1];
      float calibration = 90.0f;

      goCamera->getCameraComponent()->rotate(-angleX, -angleY - calibration);
      goCamera->updateCameraPosition();


    for( GameObjectMesh * goMesh : this->goMeshes){
        if(goMesh->getIsMovable() && goMesh->getUseGravity()){
             this->updatePhysicsMesh( goMesh, fixedStep );
        }
    if(goMesh->getIsTelekinesis()){
       goPlayer->getPlayerComponent()->setPositionChild(goPlayer,goMesh);
       QVector3D positionMesh = goMesh->getWorldPosition();
      }

    }

    // update BVH
    updateALLBVH();


    if( goPlayer->getPlayerComponent()->telekinesisActivated() ){
        goPlayer->getPlayerComponent()->castRay(goCamera->getCameraComponent()->getCameraForward());
        rayBVHCollision( this->getRoot() );
}
    // compute collision
    this->computeCollision( goPlayer );

    for( GameObjectMesh * goMesh : this->goMeshes){
        if(goMesh->getIsMovable()){
            this->computeCollision( goMesh );
        }
    }

    // remove collected collectibles
    destroyGOs();

}

void SceneGraph::destroyGOs(){
    std::vector<GameObjectMesh *>::iterator itA;

    for( GameObjectMesh * goMesh : goMeshes ){
        if( !goMesh->getIsDestroyed() )
            continue;
        qDebug() << "destroyed" << goMesh->getName().c_str();
        itA = find( goMeshes.begin(), goMeshes.end(), goMesh );
        if ( itA != goMeshes.end()  )
            goMeshes.erase( itA );
    }
}

void SceneGraph::render( GameObjectCamera * camera, Shader * shader  ){


    this->renderMesh( goPlayer, camera, shader );
    goPlayer->getPlayerComponent()->drawRay( camera->getCameraComponent()->getViewMatrix(), camera->getCameraComponent()->getProjection() );

    for( GameObjectMesh * go : this->goMeshes){
        this->renderMesh( go, camera, shader );
    }

}

void SceneGraph::renderBVH( Node * node, Shader * shader ){
    node->nodeBoundingBox.drawAABB(shader);

    if( isLeaf( node )){
        return;

    }else{

        for( Node * childNode : node->children ){
            renderBVH( childNode, shader );
        }
    }
}

void SceneGraph::updateBVH( Node * node ){

    std::vector<GameObjectMesh *>::iterator itA;

    GameObject * go = node->gameObject;
    node->nodeBoundingBox.resetAABB();

    AABB bbox;

    itA = find( goMeshes.begin(), goMeshes.end(), go);

    if ( itA != goMeshes.end()  )
        bbox = (*itA)->getMeshRenderer()->getMesh().getAABB();

     else bbox = goPlayer->getMeshRenderer()->getMesh().getAABB();


    if( go->getChildren().empty() ){
        node->nodeBoundingBox.resizeAABB( bbox );
        return;

    }else{
        node->nodeBoundingBox.resizeAABB( bbox );

        for( Node * childNode : node->children ){
            updateBVH( childNode );
            node->nodeBoundingBox.resizeAABB( childNode->nodeBoundingBox );
        }
    }
}

void SceneGraph::updateALLBVH(){
    // update children AABB
    this->root = new Node();
    for( GameObject * go : goList){
        this->root->children.push_back( buildGraphScene( go ) );
    }

    // set root AABB
    for( Node * childNode : this->root->children ){
        this->root->nodeBoundingBox.resizeAABB( childNode->nodeBoundingBox );
    }

}

void SceneGraph::rayBVHCollision( Node * node ){
    bool collision = node->nodeBoundingBox.intersect( goPlayer->getPlayerComponent()->getRay() );
    if( node->children.empty() && collision ){
        goPlayer->getPlayerComponent()->telekinesis( goPlayer, node->gameObject );

    }

    else if(collision){
        for( Node * childNode : node->children ){

            if( ( childNode->gameObject->getName() == goPlayer->getName() ) )
                continue;

            bool collision = childNode->nodeBoundingBox.intersect( goPlayer->getPlayerComponent()->getRay() );
            if( collision ){
                rayBVHCollision( childNode );
            }
        }
    }
}

bool SceneGraph::isLeaf( Node * node ){
    return node->children.empty();
}

Node * SceneGraph::getRoot(){
    return this->root;
}

