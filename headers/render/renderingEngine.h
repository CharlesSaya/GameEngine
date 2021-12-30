#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H


#include <QOpenGLFunctions_3_1>
#include <QOpenGLContext>

#include "headers/render/cubemap.h"

#include "headers/game/sceneGraph.h"

#include "headers/core/cameraComponent.h"

#include "headers/render/light.h"

#include "headers/render/shader.h"


class RenderingEngine : protected QOpenGLFunctions_3_1{

private:
    float step;
    CubeMap skybox;
    GameObjectCamera * mainCamera;
    CameraComponent *cameraOrtho;
    GameObjectCamera * cameraOrthoGO;

    uint SHADOW_WIDTH = 2048;
    uint SHADOW_HEIGHT = 2048 ;

    uint m_shadowMapFBO = 0;
    uint m_shadowMapTex = 0;
    QMatrix4x4 m_lightPositionMatrix;
    QMatrix4x4 m_lightViewMatrix;
    Light light;

    Shader * shadowShader;
    QOpenGLContext * context;
public:
    RenderingEngine();
    RenderingEngine( QOpenGLContext * context, float renderStep );

    void initializeGL();

    void configureUniforms(SceneGraph &sceneGraph);

    void renderScene( SceneGraph &sceneGraph );
    void renderOutline();

    void renderToShadowMap(SceneGraph &sceneGraph);
    void initDepthMap();

    GameObjectCamera *getMainCamera() const;
    void setMainCamera(GameObjectCamera *newMainCamera);
    const CubeMap &getSkybox() const;
    void setSkybox(const CubeMap &newSkybox);
    Light getLight();
    uint shadowMapTex() const;
};

#endif // RENDERINGENGINE_H
