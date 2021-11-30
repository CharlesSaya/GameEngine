#include "headers/game/sceneGraph.h"

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( std::vector<GameObject *>& goList, std::vector<GameObjectMesh *> &goMeshes, std::vector<GameObjectPlayer *> &goPlayers, std::vector<GameObjectCamera *> &goCameras  ){
    this->goMeshes = goMeshes;
    this->goPlayers = goPlayers;
    this->goCameras = goCameras;
    this->root = new Node();
    for( GameObject * go : goList){
        this->root->children.push_back( buildGraphScene( go ) );
    }

}

Node * SceneGraph::buildGraphScene( GameObject * go ){

    Node * node = new Node();
    node->gameObject  = go;

    std::vector<GameObjectMesh *>::iterator itA;
    std::vector<GameObjectPlayer *>::iterator itB;
    std::vector<GameObjectCamera *>::iterator itC;

    if( go->getChildren().empty() ){
        itA = find( goMeshes.begin(), goMeshes.end(), go);
        if ( itA != goMeshes.end()  ){
            node->nodeBoundingBox->resizeAABB( (*itA)->getMeshRenderer()->getMesh().getAABB() );
            return node;

        }
        itB = find( goPlayers.begin(), goPlayers.end(), go);
        if ( itB != goPlayers.end()  ){
            node->nodeBoundingBox->resizeAABB( (*itB)->getMeshRenderer()->getMesh().getAABB() );
            return node;
        }
        itC = find( goCameras.begin(), goCameras.end(), go);
        if ( itC != goCameras.end()  ){
//            node->nodeBoundingBox->resizeAABB( (*itC)->getMeshRenderer()->getMesh().getAABB() );
            return node;
        }
        return node;

    }else{

        for( GameObject * child : go->getChildren() ){
            node->children.push_back( buildGraphScene( child ) );
            node->nodeBoundingBox->resizeAABB( *node->children.back()->nodeBoundingBox );
        }

    }

    return node;
}

void SceneGraph::input(QKeyEvent *key){

}

void SceneGraph::update( float fixedStep ){
    for( GameObjectPlayer * go : this->goPlayers){
        this->updatePhysics( go, fixedStep );
    }
    for( Node * childNode : this->root->children ){
        this->updateBVH(childNode);
    }

}

void SceneGraph::render( Camera &camera  ){
    for( GameObjectMesh * go : this->goMeshes)
        this->renderMesh( go, camera );

    for( GameObjectPlayer * go : this->goPlayers)
        this->renderMesh( go, camera );
}

void SceneGraph::transformBBox( Node * node ){

    if( isLeaf(node))
        return;

    else
        for( Node * childNode : node->children )
            transformBBox( childNode );

}

void SceneGraph::updateBVH( Node * node ){

    std::vector<GameObjectMesh *>::iterator itA;
    std::vector<GameObjectPlayer *>::iterator itB;
    std::vector<GameObjectCamera *>::iterator itC;
    GameObject * go = node->gameObject;
    if( go->getChildren().empty() ){

        itA = find( goMeshes.begin(), goMeshes.end(), go);
        if ( itA != goMeshes.end()  ){
            node->nodeBoundingBox->resizeAABB( (*itA)->getMeshRenderer()->getMesh().getAABB() );
            return;

        }
        itB = find( goPlayers.begin(), goPlayers.end(), go);
        if ( itB != goPlayers.end()  ){
            node->nodeBoundingBox->resizeAABB( (*itB)->getMeshRenderer()->getMesh().getAABB() );
            qDebug() << node->nodeBoundingBox->getMax();
            return;
        }
    }

    else{
        for( Node * childNode : node->children ){
            updateBVH( childNode );
            node->nodeBoundingBox->resizeAABB(*childNode->nodeBoundingBox);
        }

    }

}


bool SceneGraph::isLeaf( Node * node ){
    return node->children.empty();
}

Node * SceneGraph::getRoot(){
    return this->root;
}
