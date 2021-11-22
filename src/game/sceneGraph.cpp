#include "headers/game/sceneGraph.h"

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( GameObject * root ){
    this->root = root;
}

GameObject * SceneGraph::getRoot(){
    return this->root;
}

void  SceneGraph::input( GameObject * gameObject, QKeyEvent * key, float deltaTime ){

}

void SceneGraph::update( GameObject * gameObject ){

}

void SceneGraph::render( GameObject * gameObject, Camera &camera){

    gameObject->render( gameObject->getModel(), camera.getViewMatrix(), camera.getProjection(), camera.getCameraPosition() );

}
