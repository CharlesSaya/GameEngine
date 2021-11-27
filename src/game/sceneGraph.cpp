#include "headers/game/sceneGraph.h"

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( std::vector<GameObject *>& goList ){

    this->root = new Node();
    for( GameObject * go : goList){
        this->root->children.push_back( buildGraphScene( go ) );
    }
}

Node * SceneGraph::buildGraphScene( GameObject * go ){

    Node * node = new Node();
    node->gameObject      = go;

    ColliderComponent * col =  static_cast<ColliderComponent *>( node->gameObject->getComponent<ColliderComponent>());
    if( col != nullptr ){
        node->nodeBoundingBox->resizeAABB( col->getMesh().getAABB() );
        node->nodeBoundingBox->transformAABB( go->getModel() );
    }

    if( go->getChildren().empty() ){

        return node;

    }else{

        for( GameObject * child : go->getChildren() ){
            node->children.push_back( buildGraphScene( child ) );
            node->nodeBoundingBox->resizeAABB( *node->children.back()->nodeBoundingBox );
        }

    }

    return node;
}

void  SceneGraph::input( QKeyEvent * key ){
     this->root->gameObject->input( key );
}

void SceneGraph::update( float fixedStep ){
    this->root->gameObject->update( fixedStep );
//    this->trandformBBox( this->root );
//    this->updateBBox( this->root );

}

void SceneGraph::render( Camera &camera ){
     this->root->gameObject->render( this->root->gameObject->getModel(), camera.getViewMatrix(), camera.getProjection(), camera.getCameraPosition() );
}

void SceneGraph::transformBBox( Node * node ){

    if( node->gameObject != nullptr )
        node->nodeBoundingBox->transformAABB( node->gameObject->getModel() );

    if( isLeaf(node))
        return;

    else
        for( Node * childNode : node->children )
            transformBBox( childNode );



}

void SceneGraph::updateBBox( Node * node ){

    if( isLeaf(node)){
        return;
    }

    else{
        for( Node * childNode : node->children ){
            updateBBox( childNode );
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
