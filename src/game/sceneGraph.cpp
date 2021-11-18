#include "headers/game/sceneGraph.h"

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( GameObject * root ){
    this->root = root;
}

GameObject * SceneGraph::getRoot(){
    return this->root;
}

void  SceneGraph::input(){


}

void SceneGraph::update(){

}

void SceneGraph::render( GameObject * gameObject, Camera &camera, QMatrix4x4 &projection ){

    gameObject->render( gameObject->getModel(), camera.getViewMatrix(), projection, camera.getCameraPosition() );

    Q_FOREACH (GameObject * g , gameObject->getChildren() ){
        render( g, camera, projection );
    }
}
