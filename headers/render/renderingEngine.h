#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>

#include "headers/render/cubemap.h"

#include "headers/game/sceneGraph.h"

#include "headers/core/gameObjectCamera.h"

#include "headers/render/vertexData.h"
#include "headers/render/pointLight.h"
#include "headers/render/renderText.h"
#include "headers/render/flareGenerator.h"
#include "headers/render/directionalLight.h"
#include "headers/render/particleGenerator.h"



#include "headers/render/shader.h"


class RenderingEngine : protected QOpenGLFunctions_3_3_Core{

private:
    float step;

    int shadowHeight = 4096;
    int shadowWidth = 4096 ;
    int screenWidth = 1920;
    int screenHeight = 1080;

    uint quadVAO = 0, quadVBO = 0;
    uint quadVAO2 = 0, quadVBO2 = 0;

    uint m_shadowMapFBO = 0;
    uint m_shadowMapTex = 0;

    uint gFBO = 0, gPosition, gNormal, gColor, gDiffuse, gBloom, depthRBO;

    uint blurVFbo = 0 , blurHFbo  = 0, blurVTexture, blurTexture;

    CubeMap skybox;
    GameObjectCamera * mainCamera;

    CameraComponent *cameraOrtho;
    GameObjectCamera * cameraOrthoGO;

    QMatrix4x4 m_lightViewMatrix;
    QMatrix4x4 m_lightPositionMatrix;

    std::vector<PointLight> pointLights;
    DirectionalLight directionalLight;

    Shader * shadowShader, * gBufferShader, *particleShader, *pointParticleShader, *postProcessShader, *flareShader, *blurVShader, *blurHShader, *textShader;

    QOpenGLContext * context;

    QVector3D white = QVector3D( 1., 1., 1.);
    QVector3D red = QVector3D( 1., 0., 0. );
    QVector3D sandColor = QVector3D( 0.76, 0.70, 0.5 );
    QVector3D emberColor = QVector3D( 0.87, 0., 0.);
    QVector3D brownColor = QVector3D( 0.5, 0.03, 0.03);
    QVector3D yellowColor = QVector3D( 242. / 255.,125. / 255.,12. / 255.);

    ParticleGenerator emberGenerator;
    ParticleGenerator emberGenerator2;
    ParticleGenerator emberGenerator3;
    ParticleGenerator sandPointGenerator;
    ParticleGenerator sandSpriteGenerator;
    ParticleGenerator leavesGenerator;
    FlareGenerator flareGenerator;

    RenderText textGenerator;

public:

    RenderingEngine();
    RenderingEngine( float renderStep );

    void initPointLights();
    void initLensFlares();
    void initParticles();
    void initTextRendering();

    void initGBufferFBO();
    void initBlurFBO();
    void initDepthFBO( SceneGraph &sceneGraph );

    void initPostProcessShader();
    void initVBlurShader();
    void initHBlurShader();

    void configureUniforms(SceneGraph &sceneGraph);

    void generateQuad();

    void renderScene( SceneGraph &sceneGraph, float deltaTime );
    void renderOutline();

    void renderShadowMap(SceneGraph &sceneGraph);
    void renderGeometryData( SceneGraph &sceneGraph );

    void renderBloom();

    void renderPostProcess();

    void renderText();

    void screenResized( int width, int height  );

    void setMainCamera(GameObjectCamera *newMainCamera);
    GameObjectCamera *getMainCamera() const;

    const CubeMap &getSkybox() const;
    void setSkybox(const CubeMap &newSkybox);

};

#endif // RENDERINGENGINE_H
