#include "headers/render/renderingEngine.h"

RenderingEngine::RenderingEngine(){

}

RenderingEngine::RenderingEngine( float renderStep ){
    this->step = renderStep;
    this->context = QOpenGLContext::currentContext();

    directionalLight = DirectionalLight(QVector3D( 10.0, 70.0, -5.0), white, white, white);
    initPointLights();
    shadowShader = new Shader(  "../GameEngine/shaders/depth_vshader.glsl", "../GameEngine/shaders/depth_fshader.glsl" );
    gBufferShader = new Shader(  "../GameEngine/shaders/gBuffer_vshader.glsl", "../GameEngine/shaders/gBuffer_fshader.glsl" );
    postProcessShader = new Shader(  "../GameEngine/shaders/postProcess_vshader.glsl", "../GameEngine/shaders/postProcess_fshader.glsl" );
    particleShader = new Shader(  "../GameEngine/shaders/particle_vshader.glsl", "../GameEngine/shaders/particle_fshader.glsl" );

    cameraOrtho  = new CameraComponent( directionalLight.getLightPosition(), QVector3D(0.0f,0.0f,0.0f),-50.0f,50.0f,-50.0f,50.0f,0.01f,100.f );
    cameraOrtho->setProjectionOrtho();
    cameraOrthoGO = new GameObjectCamera("camera ortho", cameraOrtho);


    initializeOpenGLFunctions();


}

void RenderingEngine::initializeGL(){

}

void RenderingEngine::initPointLights(){
    const unsigned int NR_LIGHTS = 1;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        float yPos = 5.;
        float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        pointLights.push_back( PointLight( 1.0, 0.1, 0.032, QVector3D( xPos, yPos, zPos), white, white, white) );

    }
}

void RenderingEngine::initGBufferFBO()
{
    if( gFBO != 0 )
        return;

    glGenFramebuffers(1, &gFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

    // - position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - color + specular color buffer
    glGenTextures(1, &gDiffuse);
    glBindTexture(GL_TEXTURE_2D, gDiffuse);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuse, 0);

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers( 1, &depthRBO );
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());

}

void RenderingEngine::initDepthMap( SceneGraph &sceneGraph )
{
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

    // Cleanup for now.
    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    configureUniforms( sceneGraph );

}


void RenderingEngine::initPostProcessShader(){

    postProcessShader->useShaderProgram();

    postProcessShader->setUniformValue( "cameraPosition", mainCamera->getCameraComponent()->getCameraPosition() );
    postProcessShader->setUniformValue( "lightSpaceMatrix", cameraOrtho->getProjection() * cameraOrtho->getViewMatrix() );
    postProcessShader->loadDirectionalLight(directionalLight);
    postProcessShader->loadPointLights(pointLights);

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, gDiffuse );
    postProcessShader->setUniformValue("diffuseTexture", 0);

    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, m_shadowMapTex   );
    postProcessShader->setUniformValue("shadowTexture", 1);

    glActiveTexture( GL_TEXTURE2 );
    glBindTexture( GL_TEXTURE_2D, gNormal );
    postProcessShader->setUniformValue("normalTexture", 2);

    glActiveTexture( GL_TEXTURE3 );
    glBindTexture( GL_TEXTURE_2D, gPosition );
    postProcessShader->setUniformValue("positionTexture", 3);

}

void RenderingEngine::renderShadowMap(SceneGraph &sceneGraph)
{
    initDepthMap( sceneGraph );

    // TODO : I should try deleting this line
    shadowShader->useShaderProgram();

    // Render into the depth framebuffer
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        sceneGraph.render(cameraOrthoGO, shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

}


void RenderingEngine::renderGeometryData( SceneGraph &sceneGraph ){

    initGBufferFBO();
    gBufferShader->useShaderProgram();

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        sceneGraph.render( mainCamera, gBufferShader );
    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

}

void RenderingEngine::renderPostProcess()
{
    initPostProcessShader();

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderingEngine::renderScene( SceneGraph &sceneGraph,  float deltaTime ){

    renderShadowMap(sceneGraph);
    renderGeometryData(sceneGraph);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    renderPostProcess();

    // blit gbuffer depth render buffer object to defaut framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, context->defaultFramebufferObject()); // write to default framebuffer
    glBlitFramebuffer( 0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
    skybox.render( mainCamera, QMatrix4x4() );

    particleGenerator.update( deltaTime );
    particleGenerator.render( particleShader );
}


void RenderingEngine::configureUniforms( SceneGraph &sceneGraph ){
    for( GameObjectMesh * go : sceneGraph.getGoMeshes()){
        go->getMeshRenderer()->getMesh().setShadowTexture( m_shadowMapTex );
        go->getMeshRenderer()->getMesh().getShader()->loadDirectionalLight(directionalLight);
        go->getMeshRenderer()->getMesh().getShader()->loadPointLights(pointLights);
        go->getMeshRenderer()->setLightSpaceMatrix( cameraOrtho->getProjection() * cameraOrtho->getViewMatrix() );
    }

    for( GameObjectPlayer * go : sceneGraph.getGoPlayers()){
        go->getMeshRenderer()->getMesh().setShadowTexture( m_shadowMapTex );
        go->getMeshRenderer()->getMesh().getShader()->loadDirectionalLight(directionalLight);
        go->getMeshRenderer()->getMesh().getShader()->loadPointLights(pointLights);
        go->getMeshRenderer()->setLightSpaceMatrix( cameraOrtho->getProjection() * cameraOrtho->getViewMatrix() );
    }
}



GameObjectCamera *RenderingEngine::getMainCamera() const
{
    return mainCamera;
}

void RenderingEngine::setMainCamera(GameObjectCamera *newMainCamera)
{
    mainCamera = newMainCamera;

    Texture sprite = Texture( "../GameEngine/textures/skybox/MusicHall/py.png", "sprite" );

    particleGenerator = ParticleGenerator( 1000, sprite, mainCamera );
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



