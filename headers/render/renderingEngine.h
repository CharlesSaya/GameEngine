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
#include "headers/render/flareGenerator.h"
#include "headers/render/pointLight.h"


#include "headers/render/shader.h"


class RenderingEngine : protected QOpenGLFunctions_3_3_Core{

private:
    float step;

    int SHADOW_WIDTH = 2048;
    int SHADOW_HEIGHT = 2048 ;
    int SCR_WIDTH = 1920;
    int SCR_HEIGHT = 1080;

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

    Shader * shadowShader, * gBufferShader, *particleShader, *postProcessShader, *flareShader;

    QOpenGLContext * context;

    QVector3D white = QVector3D( 1., 1., 1.);

    ParticleGenerator particleGenerator;
    FlareGenerator flareGenerator;

public:

    RenderingEngine();
    RenderingEngine( float renderStep );

    void initGBufferFBO();
    void initializeGL();
    void initDepthMap( SceneGraph &sceneGraph );
    void initPointLights();
    void initPostProcessShader();
    void initLensFlares();
    void initParticles();
    void generateQuad();

    void configureUniforms(SceneGraph &sceneGraph);

    void renderScene( SceneGraph &sceneGraph, float deltaTime );
    void renderOutline();

    void renderShadowMap(SceneGraph &sceneGraph);
    void renderGeometryData( SceneGraph &sceneGraph );

    void renderPostProcess();

    void screenResized( int width, int height  );

    void setMainCamera(GameObjectCamera *newMainCamera);
    GameObjectCamera *getMainCamera() const;

    const CubeMap &getSkybox() const;
    void setSkybox(const CubeMap &newSkybox);

    uint shadowMapTex() const;
};

#endif // RENDERINGENGINE_H
