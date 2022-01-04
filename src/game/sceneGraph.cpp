#include "headers/game/sceneGraph.h"

const std::vector<GameObjectMesh *> &SceneGraph::getGoMeshes() const
{
    return goMeshes;
}

const std::vector<GameObjectPlayer *> &SceneGraph::getGoPlayers() const
{
    return goPlayers;
}

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( std::vector<GameObject *>& goList,
                        std::vector<GameObjectMesh *> &goMeshes,
                        std::vector<GameObjectPlayer *> &goPlayers,
                        std::vector<GameObjectCamera *> &goCameras,
                        PhysicsEngine& physicsEngine,
                        ColliderEngine & colliderEngine  ){

    this->goList = goList;
    this->goMeshes = goMeshes;
    this->goPlayers = goPlayers;
    this->goCameras = goCameras;
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
    std::vector<GameObjectPlayer *>::iterator itB;

    AABB bbox;
    itA = find( goMeshes.begin(), goMeshes.end(), go);
    itB = find( goPlayers.begin(), goPlayers.end(), go);

    if ( itA != goMeshes.end()  )
        bbox = (*itA)->getMeshRenderer()->getMesh().getAABB();

    else if ( itB != goPlayers.end()  )
        bbox = (*itB)->getMeshRenderer()->getMesh().getAABB();

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
    for( GameObjectPlayer * go : this->goPlayers){
        this->updatePhysics( go, fixedStep );
        go->rotate( go->getMoveComponent()->getRotationY()* go->getMoveComponent()->getRotationX().inverted() );
    }

    // update main camera position
    for( GameObjectCamera * goC : this->goCameras){
        QQuaternion parentRotationX = dynamic_cast<GameObjectPlayer*>(goC->getParent())->getMoveComponent()->getRotationX();
        float angleX = parentRotationX.toEulerAngles()[0];

        QQuaternion parentRotationY = dynamic_cast<GameObjectPlayer*>(goC->getParent())->getMoveComponent()->getRotationY();
        float angleY = parentRotationY.toEulerAngles()[1];

        float calibration = 90.0f;

        goC->getCameraComponent()->rotate(-angleX, -angleY - calibration);
        goC->updateCameraPosition();
    }

    for( GameObjectMesh * goMesh : this->goMeshes){
        if(goMesh->getIsMovable() && goMesh->getUseGravity()){
            qDebug()<< goMesh->getTransform()->getPosition();
             this->updatePhysicsMesh( goMesh, fixedStep );
        }


    }
    // update BVH
    updateALLBVH();


    for( GameObjectPlayer * go : this->goPlayers){
        if( ! go->getPlayerComponent()->telekinesisActivated() )
            continue;

        go->getPlayerComponent()->castRay();
        rayBVHCollision( go, this->getRoot() );
    }

    // compute collision
    for( GameObjectPlayer * go : this->goPlayers){
        this->computeCollision( go );
    }
}


void SceneGraph::render( GameObjectCamera * camera, Shader * shader  ){
    for( GameObjectMesh * go : this->goMeshes){
         this->renderMesh( go, camera, shader );
    }

    for( GameObjectPlayer * go : this->goPlayers){
        this->renderMesh( go, camera, shader );
        go->getPlayerComponent()->drawRay( camera->getCameraComponent()->getViewMatrix(), camera->getCameraComponent()->getProjection() );
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
    std::vector<GameObjectPlayer *>::iterator itB;
    std::vector<GameObjectCamera *>::iterator itC;

    GameObject * go = node->gameObject;
    node->nodeBoundingBox.resetAABB();

    AABB bbox;

    itA = find( goMeshes.begin(), goMeshes.end(), go);
    itB = find( goPlayers.begin(), goPlayers.end(), go);

    if ( itA != goMeshes.end()  )
        bbox = (*itA)->getMeshRenderer()->getMesh().getAABB();

    else if ( itB != goPlayers.end()  )
        bbox = (*itB)->getMeshRenderer()->getMesh().getAABB();


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

void SceneGraph::rayBVHCollision( GameObjectPlayer * playerGO, Node * node ){
    bool collision = node->nodeBoundingBox.intersect( playerGO->getPlayerComponent()->getRay() );

    if( node->children.empty() && collision ){
        playerGO->getPlayerComponent()->telekinesis( playerGO, node->gameObject );

    }

    else if(collision){
        for( Node * childNode : node->children ){

            if( ( childNode->gameObject->getName() == playerGO->getName() ) )
                continue;

            bool collision = childNode->nodeBoundingBox.intersect( playerGO->getPlayerComponent()->getRay() );
            if( collision ){
                rayBVHCollision( playerGO, childNode );
            }
             playerGO->getPlayerComponent()->attractAndPush(playerGO);
        }
    }
}

bool SceneGraph::isLeaf( Node * node ){
    return node->children.empty();
}

Node * SceneGraph::getRoot(){
    return this->root;
}
