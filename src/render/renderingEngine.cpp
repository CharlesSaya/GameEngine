#include "headers/render/renderingEngine.h"

RenderingEngine::RenderingEngine(){

}

RenderingEngine::RenderingEngine( float renderStep ){
    this->step = renderStep;
    this->context = QOpenGLContext::currentContext();

    directionalLight = DirectionalLight(QVector3D(64.0, 60.0, 64.0), white);

    initPointLights();

    shadowShader = new Shader(  "../GameEngine/shaders/depth_vshader.glsl", "../GameEngine/shaders/depth_fshader.glsl" );
    gBufferShader = new Shader(  "../GameEngine/shaders/gBuffer_vshader.glsl", "../GameEngine/shaders/gBuffer_fshader.glsl" );
    postProcessShader = new Shader(  "../GameEngine/shaders/postProcess_vshader.glsl", "../GameEngine/shaders/postProcess_fshader.glsl" );
    particleShader = new Shader(  "../GameEngine/shaders/particle_vshader.glsl", "../GameEngine/shaders/particle_fshader.glsl" );
    pointParticleShader = new Shader(  "../GameEngine/shaders/particlePoint_vshader.glsl", "../GameEngine/shaders/particlePoint_fshader.glsl" );
    flareShader = new Shader(  "../GameEngine/shaders/flare_vshader.glsl", "../GameEngine/shaders/flare_fshader.glsl" );

    blurVShader = new Shader(  "../GameEngine/shaders/verticalBlur_vshader.glsl", "../GameEngine/shaders/blur_fshader.glsl" );
    blurHShader = new Shader(  "../GameEngine/shaders/horizontalBlur_vshader.glsl", "../GameEngine/shaders/blur_fshader.glsl" );

    cameraOrtho  = new CameraComponent( directionalLight.getLightPosition(), QVector3D(64.0f,0.0f,64.0f),-60.0f,60.0f,-60.0f,60.0f,0.01f,150.f );
    cameraOrtho->setProjectionOrtho();
    cameraOrthoGO = new GameObjectCamera("camera ortho", cameraOrtho);

    initializeOpenGLFunctions();

    initLensFlares();
    initParticles();
}

// INIT POINT LIGHTS

void RenderingEngine::initPointLights(){
    const unsigned int NR_LIGHTS = 1;

    srand(1000);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        float xPos = ((rand() % 100) / 100.0) * 5.0;
        float yPos = 5.;
        float zPos = ((rand() % 100) / 100.0) * 5.0;
        pointLights.push_back( PointLight( 1.0, 0.1, 0.032, QVector3D( xPos, yPos, zPos), red) );

    }
}

// CONFIGURE PARTICLES

void RenderingEngine::initParticles(){

    // SNOW PARTICLES

    Texture snowSprite = Texture( "../GameEngine/textures/snowflakes.png", "sprite" );

    snowGenerator = ParticleGenerator( 5000, QVector3D( 0.0, 0., 97 ), QVector3D( 1.0, 0.0, 0.0 ), snowColor,  true );
    snowGenerator.setRange( 31 );
    // SAND PARTICLES

    sandPointGenerator = ParticleGenerator( 2500, QVector3D( 0.0, 0., 52. ), QVector3D( 1.0, 0.0, 0.0 ), sandColor, true );
    sandPointGenerator.setRange( 13 );

    Texture sandSprite = Texture( "../GameEngine/textures/sand.png", "sprite" );
    sandSpriteGenerator = ParticleGenerator( 2500, sandSprite, QVector3D( 0.0, 0., 52. ), QVector3D( 1.0, 0.0, 0.0 ), false );
    sandSpriteGenerator.setRange( 18 );

    // LEAVES PARTICLES

    Texture leafSprite = Texture( "../GameEngine/textures/snowflakes.png", "sprite" );

    leavesGenerator = ParticleGenerator( 1000, leafSprite, QVector3D( 0.0, 0., 19 ), QVector3D( 1.0, -0.0, 0.0 ), false );
    leavesGenerator.setRange( 18 );

}
// CONFIGURE LENS FLARE

void RenderingEngine::initLensFlares(){

    Texture sun      = Texture( "../GameEngine/textures/lensFlare/sun.png", "flare" );
    Texture texture2 = Texture( "../GameEngine/textures/lensFlare/tex2.png", "flare" );
    Texture texture3 = Texture( "../GameEngine/textures/lensFlare/tex3.png", "flare" );
    Texture texture4 = Texture( "../GameEngine/textures/lensFlare/tex4.png", "flare" );
    Texture texture5 = Texture( "../GameEngine/textures/lensFlare/tex5.png", "flare" );
    Texture texture6 = Texture( "../GameEngine/textures/lensFlare/tex6.png", "flare" );
    Texture texture7 = Texture( "../GameEngine/textures/lensFlare/tex7.png", "flare" );
    Texture texture8 = Texture( "../GameEngine/textures/lensFlare/tex8.png", "flare" );

    std::vector<FlareTexture> flareTextures  ={ FlareTexture( sun, 0.15),
                                                FlareTexture( texture2, 0.1),
                                                FlareTexture( texture3, 0.1),
                                                FlareTexture( texture4, 0.1),
                                                FlareTexture( texture5, 0.1),
                                                FlareTexture( texture6, 0.1),
                                                FlareTexture( texture7, 0.1),
                                                FlareTexture( texture8, 0.1) };

    flareGenerator = FlareGenerator( directionalLight, flareTextures );

}

void RenderingEngine::generateQuad(){

    if (quadVAO == 0)
     {
         float quadVertices[] = {
             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
              1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         };

         glGenVertexArrays(1, &quadVAO);

         glGenBuffers(1, &quadVBO);
         glBindVertexArray(quadVAO);

         glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

     }

}

// CONGIFURE FBO

void RenderingEngine::initGBufferFBO()
{
    if( gFBO != 0 )
        return;

    // initialize Gbuffer FBO to render geometry data in a first pass

    glGenFramebuffers(1, &gFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

    // position  buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // normal  buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // color buffer
    glGenTextures(1, &gDiffuse);
    glBindTexture(GL_TEXTURE_2D, gDiffuse);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuse, 0);

    // bloom buffer
    glGenTextures(1, &gBloom);
    glBindTexture(GL_TEXTURE_2D, gBloom);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBloom, 0);

    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    // depth render buffer
    glGenRenderbuffers( 1, &depthRBO );
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());

}

void RenderingEngine::initBlurFBO()
{
    if( blurVFbo == 0 ){

        glGenFramebuffers(1, &blurVFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, blurVFbo);

        glGenTextures(1, &blurVTexture);
        glBindTexture(GL_TEXTURE_2D, blurVTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurVTexture, 0);

        glDrawBuffer( GL_COLOR_ATTACHMENT0 );

        glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    }

    if( blurHFbo == 0 ){

        glGenFramebuffers(1, &blurHFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, blurHFbo);

        glGenTextures(1, &blurTexture);
        glBindTexture(GL_TEXTURE_2D, blurTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);

        glDrawBuffer( GL_COLOR_ATTACHMENT0 );

        glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    }

}

void RenderingEngine::initDepthFBO( SceneGraph &sceneGraph )
{
    if(m_shadowMapFBO != 0)
        return;

    // Create a texture for storing the depth map
    glGenTextures(1, &m_shadowMapTex);
    glBindTexture(GL_TEXTURE_2D, m_shadowMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadowHeight, shadowWidth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Create a FBO and attach the depth texture
    glGenFramebuffers(1, &m_shadowMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapTex, 0);

    glDrawBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    configureUniforms( sceneGraph );

}

// CONGIFURE SHADERS

void RenderingEngine::initPostProcessShader(){

    postProcessShader->useShaderProgram();

    postProcessShader->loadPointLights(pointLights);
    postProcessShader->loadDirectionalLight(directionalLight);
    postProcessShader->setUniformValue( "cameraPosition", mainCamera->getCameraComponent()->getCameraPosition() );
    postProcessShader->setUniformValue( "lightSpaceMatrix", cameraOrtho->getProjection() * cameraOrtho->getViewMatrix() );
    postProcessShader->setUniformValue( "view", mainCamera->getCameraComponent()->getViewMatrix() );

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

    glActiveTexture( GL_TEXTURE4 );
    glBindTexture( GL_TEXTURE_2D, gBloom );
    postProcessShader->setUniformValue("bloomTexture", 4);

}

void RenderingEngine::initVBlurShader(){

    blurVShader->useShaderProgram();

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, gBloom );
    blurVShader->setUniformValue("target", 0);
}

void RenderingEngine::initHBlurShader(){

    blurHShader->useShaderProgram();

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, blurVTexture );
    blurHShader->setUniformValue("target", 0);

}


void RenderingEngine::configureUniforms( SceneGraph &sceneGraph ){
    for( GameObjectMesh * go : sceneGraph.getGoMeshes()){
        go->getMeshRenderer()->getMesh().setShadowTexture( m_shadowMapTex );
        go->getMeshRenderer()->getMesh().getShader()->loadDirectionalLight(directionalLight);
        go->getMeshRenderer()->getMesh().getShader()->loadPointLights(pointLights);
        go->getMeshRenderer()->setLightSpaceMatrix( cameraOrtho->getProjection() * cameraOrtho->getViewMatrix() );
    }

    GameObjectPlayer *goPlayer = sceneGraph.getGoPlayer();
        goPlayer->getMeshRenderer()->getMesh().setShadowTexture( m_shadowMapTex );
        goPlayer->getMeshRenderer()->getMesh().getShader()->loadDirectionalLight(directionalLight);
        goPlayer->getMeshRenderer()->getMesh().getShader()->loadPointLights(pointLights);
        goPlayer->getMeshRenderer()->setLightSpaceMatrix( cameraOrtho->getProjection() * cameraOrtho->getViewMatrix() );

}

// RENDERING FUNCTIONS

void RenderingEngine::renderBloom(){
    initBlurFBO();
    initVBlurShader();

    // Render Vertical blur

    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, blurVFbo);

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

    // Render Horizontal blur

    blurHShader->useShaderProgram();

    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, blurHFbo);

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    glViewport(0, 0, screenWidth, screenHeight);

}

void RenderingEngine::renderShadowMap(SceneGraph &sceneGraph)
{

    initDepthFBO( sceneGraph );

    shadowShader->useShaderProgram();

    // Render in depth FBO
    glViewport(0, 0, shadowHeight, shadowWidth);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        sceneGraph.render(cameraOrthoGO, shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    glViewport(0, 0, screenWidth, screenHeight);

}

void RenderingEngine::renderGeometryData( SceneGraph &sceneGraph ){

    initGBufferFBO();
    gBufferShader->useShaderProgram();

    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        sceneGraph.render( mainCamera, gBufferShader );

    glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
    glViewport(0, 0, screenWidth, screenHeight);

}

void RenderingEngine::renderPostProcess()
{

    initPostProcessShader();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderingEngine::renderScene( SceneGraph &sceneGraph,  float deltaTime ){

    renderShadowMap(sceneGraph);
    renderGeometryData(sceneGraph);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//    configureUniforms( sceneGraph );
//    sceneGraph.render( mainCamera );

    // render the final scene by adding the differents FBO textures on a quad
    generateQuad();

//    renderBloom();

    renderPostProcess();

    // blit gbuffer depth render buffer object to defaut framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, context->defaultFramebufferObject());
    glBlitFramebuffer( 0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST );

    // render skybox
    skybox.render( mainCamera, QMatrix4x4() );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

//    snowGenerator.update( deltaTime );
//    snowGenerator.render( pointParticleShader );

//    sandSpriteGenerator.update( deltaTime );
//    sandSpriteGenerator.render( particleShader );

//    sandPointGenerator.update( deltaTime );
//    sandPointGenerator.render( pointParticleShader );

//    leavesGenerator.update( deltaTime );
//    leavesGenerator.render( particleShader );

    // render lens flare effect
    glDisable(GL_DEPTH_TEST);

    flareGenerator.render( 0.8f, flareShader );


    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

}

GameObjectCamera *RenderingEngine::getMainCamera() const
{
    return mainCamera;
}

void RenderingEngine::setMainCamera(GameObjectCamera *newMainCamera)
{
    mainCamera = newMainCamera;
    snowGenerator.setCamera( mainCamera );
    sandPointGenerator.setCamera( mainCamera );
    sandSpriteGenerator.setCamera( mainCamera );
    leavesGenerator.setCamera( mainCamera );
    flareGenerator.setCamera( mainCamera );
}

const CubeMap &RenderingEngine::getSkybox() const
{
    return skybox;
}

void RenderingEngine::setSkybox(const CubeMap &newSkybox)
{
    skybox = newSkybox;
}

void RenderingEngine::screenResized( int width, int height ){
    screenWidth = width;
    screenHeight = height;

    flareGenerator.screenResized( width, height );
}
