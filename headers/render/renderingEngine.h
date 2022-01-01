#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>

#include "headers/render/cubemap.h"

#include "headers/game/sceneGraph.h"

#include "headers/core/gameObjectCamera.h"

#include "headers/render/directionalLight.h"
#include "headers/render/particleGenerator.h"
#include "headers/render/pointLight.h"


#include "headers/render/shader.h"


class RenderingEngine : protected QOpenGLFunctions_3_3_Core{

private:
    float step;

    uint SHADOW_WIDTH = 2048;
    uint SHADOW_HEIGHT = 2048 ;
    uint SCR_WIDTH = 1920;
    uint SCR_HEIGHT = 1080;

    uint quadVAO = 0, quadVBO = 0;

    uint m_shadowMapFBO = 0;
    uint m_shadowMapTex = 0;

    uint gFBO, gPosition, gNormal, gColorSpec, gDiffuse, depthRBO;

    CubeMap skybox;
    GameObjectCamera * mainCamera;

    CameraComponent *cameraOrtho;
    GameObjectCamera * cameraOrthoGO;

    QMatrix4x4 m_lightViewMatrix;
    QMatrix4x4 m_lightPositionMatrix;

    std::vector<PointLight> pointLights;
    DirectionalLight directionalLight;

    Shader * shadowShader, * gBufferShader, * particleShader, *postProcessShader;
    QOpenGLContext * context;

    QVector3D white = QVector3D( 1., 1., 1.);

    ParticleGenerator particleGenerator;

public:

    RenderingEngine();
    RenderingEngine( float renderStep );

    void initGBufferFBO();
    void initializeGL();
    void initDepthMap( SceneGraph &sceneGraph );
    void initPointLights();
    void initPostProcessShader();

    void configureUniforms(SceneGraph &sceneGraph);

    void renderScene( SceneGraph &sceneGraph, float deltaTime );
    void renderOutline();

    void renderShadowMap(SceneGraph &sceneGraph);
    void renderGeometryData( SceneGraph &sceneGraph );

    void renderPostProcess();

    void setMainCamera(GameObjectCamera *newMainCamera);
    GameObjectCamera *getMainCamera() const;

    const CubeMap &getSkybox() const;
    void setSkybox(const CubeMap &newSkybox);

    uint shadowMapTex() const;
};

#endif // RENDERINGENGINE_H
