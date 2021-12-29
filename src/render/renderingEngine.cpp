#include "headers/render/renderingEngine.h"





RenderingEngine::RenderingEngine(){

}

RenderingEngine::RenderingEngine( QOpenGLContext * context, float renderStep ){
    this->step = renderStep;
    this->mainCamera = mainCamera;
    light = Light(QVector3D( 5.0, 4.0, -5.0));
    shadowShader = new Shader(  "../GameEngine/shaders/shadow_vshader.glsl", "../GameEngine/shaders/shadow_fshader.glsl" );
    cameraOrtho = new CameraComponent(light.getLightPosition(), QVector3D(0.0f,0.0f,0.0f),-10.0f,10.0f,-10.0f,10.0f,0.0f,10.5f);
    cameraOrthoGO = new GameObjectCamera("camera ortho", cameraOrtho);
    this->context = context;
    initializeOpenGLFunctions();

}

void RenderingEngine::initializeGL(){

    //    glEnable(GL_DEPTH_TEST);
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    //    glEnable(GL_CULL_FACE);
}


void RenderingEngine::initDepthMap()
{
    // Refer http://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
    if(m_shadowMapFBO != 0)
        return;

    // Create a texture for storing the depth map
    glGenTextures(1, &m_shadowMapTex);
    glBindTexture(GL_TEXTURE_2D, m_shadowMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Create a frame-buffer and associate the texture with it.
    glGenFramebuffers(1, &m_shadowMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapTex, 0);

    // Let OpenGL know that we are not interested in colors for this buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Cleanup for now.
    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
}


void RenderingEngine::renderToShadowMap(SceneGraph sceneGraph)
{
    initDepthMap();

    // Render into the depth framebuffer
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        sceneGraph.render(cameraOrthoGO, shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    glViewport(0, 0, 1920, 1080);

}

void RenderingEngine::renderScene( SceneGraph sceneGraph ){
    GLint drawFboId = 0, readFboId = 0;
//    renderToShadowMap(sceneGraph);

//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    skybox.render( this->mainCamera, QMatrix4x4());
    sceneGraph.render( this->mainCamera);
}

GameObjectCamera *RenderingEngine::getMainCamera() const
{
    return mainCamera;
}

void RenderingEngine::setMainCamera(GameObjectCamera *newMainCamera)
{
    mainCamera = newMainCamera;
}

uint RenderingEngine::shadowMapTex() const
{
    return m_shadowMapTex;
}


const CubeMap &RenderingEngine::getSkybox() const
{
    return skybox;
}

void RenderingEngine::setSkybox(const CubeMap &newSkybox)
{
    skybox = newSkybox;
}

Light RenderingEngine::getLight()
{
    return light;
}
