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

void SceneGraph::render( GameObject * gameObject, QMatrix4x4 &view, QMatrix4x4 &projection, QOpenGLShaderProgram * program ){

    program->setUniformValue("mvp_matrix", projection * view * gameObject->getModel());

    if ( gameObject->hasMesh()){
        gameObject->render();
    }

    Q_FOREACH (GameObject * g , gameObject->getChildren() ){
        render( g, view, projection, program );
    }
}
