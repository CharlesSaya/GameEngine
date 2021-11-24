#include "headers/game/sceneGraph.h"

SceneGraph::SceneGraph(){

}

SceneGraph::SceneGraph( GameObject * root ){
    this->root = root;
}

GameObject * SceneGraph::getRoot(){
    return this->root;
}

void  SceneGraph::input( QKeyEvent * key ){
    this->root->input( key );
}

void SceneGraph::update( float fixedStep ){
    this->root->update( fixedStep );
}

void SceneGraph::render(  Camera &camera){
    this->root->render( root->getModel(),
                        camera.getViewMatrix(),
                        camera.getProjection(),
                        camera.getCameraPosition()
                      );
}
