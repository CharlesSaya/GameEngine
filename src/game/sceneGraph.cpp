#include "headers/game/sceneGraph.h"


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
    timer.start();
}

/*
 * Contruct the scene graph
 */
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


/*
 * Update the physics, position and BVH gameObjects
 * Compute Collision
 */
void SceneGraph::update( float fixedStep ){
    // update player physics
      this->updatePhysics( goPlayer, fixedStep );
      goPlayer->rotate(goPlayer->getMoveComponent()->getRotationY());
//      qDebug()<< goPlayer->getPhysicsComponent()->getVelocity();

      // update main camera position
      float angleX = goCamera->getMoveComponent()->getRotationX().toEulerAngles()[0];

      QQuaternion parentRotation = dynamic_cast<GameObjectPlayer*>(goCamera->getParent())->getMoveComponent()->getRotationY();
      float angleY = parentRotation.toEulerAngles()[1];
      float calibration = 90.0f;

      goCamera->getCameraComponent()->rotate(-angleX, -angleY - calibration);
      goCamera->updateCameraPosition();


    for( GameObjectMesh * goMesh : this->goMeshes){

        checkCollectibleNumber(goMesh);
        if(goMesh->getIsMovable() && goMesh->getUseGravity()){
             this->updatePhysicsMesh( goMesh, fixedStep );
        }
        if(goMesh->getIsTelekinesis()){
           goPlayer->getPlayerComponent()->setPositionChild(goPlayer,goMesh);
           QVector3D positionMesh = goMesh->getWorldPosition();
        }
        if(goMesh->getName().find("Ring")!=string::npos){
            rotateRing(goMesh);
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

    colliderEngine.updatePlayerCurrentCollisionsList( goPlayer );

    // remove collected collectibles
    destroyGOs();

}
/*
 * Destroys gameObjects
 */
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

/*
 * Render the scene Graph
 */
void SceneGraph::render( GameObjectCamera * camera, Shader * shader  ){


    this->renderMesh( goPlayer, camera, shader );
    goPlayer->getPlayerComponent()->drawRay( camera->getCameraComponent()->getViewMatrix(), camera->getCameraComponent()->getProjection() );

    for( GameObjectMesh * go : this->goMeshes){
        this->renderMesh( go, camera, shader );
    }

}

/*
 * Render BVH
 */
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

/*
 * Update the position of player
 */
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

/*
 * detect collision ray/BVH collision
 */
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

/*
 * return true if node has no child
 */
bool SceneGraph::isLeaf( Node * node ){
    return node->children.empty();
}

/*
 * Check the number of collectible in player inventory
 * If there is a certain quantity, open grids
 */
void SceneGraph::checkCollectibleNumber(GameObjectMesh *grid)
{

    if( grid->getName()=="Grid0" && goPlayer->getPlayerComponent()->getCollectibleNumber()>=1){
        elapsedTime = timer.elapsed() -elapsedTime;
        grid->move(0.0f,gridSpeed*elapsedTime,0.0f);
        if(!gridSoundPlayed){
            SoundEngine().grid();
            gridSoundPlayed=true;
        }
        if(grid->getWorldPosition().y()<-10.0f){
            grid->destroy();
            elapsedTime = 0;

        }
    }
    if( grid->getName()=="Grid1" && goPlayer->getPlayerComponent()->getCollectibleNumber()>=2){
        elapsedTime = timer.elapsed() -elapsedTime;
        grid->move(0.0f,gridSpeed*elapsedTime,0.0f);
        if(!gridSoundPlayed){
            SoundEngine().grid();
            gridSoundPlayed=true;
        }
        if(grid->getWorldPosition().y()<-10.0f){
            grid->destroy();
            elapsedTime = 0;
            gridSoundPlayed = false;
        }
    }
    if( grid->getName()=="Grid2"&& goPlayer->getPlayerComponent()->getCollectibleNumber()>=3){
        elapsedTime = timer.elapsed() -elapsedTime;
        grid->move(0.0f,gridSpeed*elapsedTime,0.0f);
        if(!gridSoundPlayed){
            SoundEngine().grid();
            gridSoundPlayed=true;
        }
        if(grid->getWorldPosition().y()<-20.0f){
            grid->destroy();
            gridSoundPlayed = false;
        }
    }


}


void SceneGraph::rotateRing(GameObjectMesh *ring)
{
    ring->rotate(QVector3D(0.0f,1.0f,0.0f),timer.elapsed()/10);
}

const std::vector<GameObjectMesh *> &SceneGraph::getGoMeshes() const
{
    return goMeshes;
}

GameObjectPlayer *SceneGraph::getGoPlayer() const
{
    return goPlayer;
}

Node * SceneGraph::getRoot(){
    return this->root;
}
