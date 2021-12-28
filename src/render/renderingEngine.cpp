#include "headers/render/renderingEngine.h"



RenderingEngine::RenderingEngine(){

}

RenderingEngine::RenderingEngine( float renderStep ){
    this->step = renderStep;
    this->mainCamera = mainCamera;
}

void RenderingEngine::initializeGL(){
    initializeOpenGLFunctions();

//    glEnable(GL_DEPTH_TEST);
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    //    glEnable(GL_CULL_FACE);
}

void RenderingEngine::renderScene( SceneGraph sceneGraph ){

    skybox.render( this->mainCamera, QMatrix4x4() );
    sceneGraph.render( this->mainCamera );
}

GameObjectCamera *RenderingEngine::getMainCamera() const
{
    return mainCamera;
}

void RenderingEngine::setMainCamera(GameObjectCamera *newMainCamera)
{
    mainCamera = newMainCamera;
}


const CubeMap &RenderingEngine::getSkybox() const
{
    return skybox;
}

void RenderingEngine::setSkybox(const CubeMap &newSkybox)
{
    skybox = newSkybox;
}
