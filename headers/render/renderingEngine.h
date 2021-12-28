#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H


#include <QOpenGLFunctions_3_1>

#include "headers/render/cubemap.h"

#include "headers/game/sceneGraph.h"

#include "headers/core/gameObjectCamera.h"

class RenderingEngine : protected QOpenGLFunctions_3_1{

private:
    float step;
    CubeMap skybox;
    GameObjectCamera * mainCamera;
public:
    RenderingEngine();
    RenderingEngine( float renderStep );

    void initializeGL();

    void renderScene( SceneGraph  sceneGraph );
    void renderShadow();
    void renderOutline();

    GameObjectCamera *getMainCamera() const;
    void setMainCamera(GameObjectCamera *newMainCamera);
    const CubeMap &getSkybox() const;
    void setSkybox(const CubeMap &newSkybox);
};

#endif // RENDERINGENGINE_H
