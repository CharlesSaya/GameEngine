#include "headers/game/game.h"

Game::Game( QObject * parent ) : QObject(parent){

}

Game::Game( CameraComponent * camera,  QObject * parent ) : QObject(parent){
    this->camera = camera;
}

void Game::initGame(){

    // Shaders & Lights  --------------------------------------------------------------------------------

    shader = new Shader( "../GameEngine/shaders/base_vshader.glsl", "../GameEngine/shaders/base_fshader.glsl" );
    terrainShader = new Shader(  "../GameEngine/shaders/terrain_vshader.glsl", "../GameEngine/shaders/terrain_fshader.glsl" );
    skyboxShader = new Shader(  "../GameEngine/shaders/skybox_vshader.glsl", "../GameEngine/shaders/skybox_fshader.glsl" );

    // Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere/";
    std::string bunnyObj  = "../GameEngine/objects/bunny/";
    std::string treeObj  = "../GameEngine/objects/tree/";
    std::string terrainOBJ  = "../GameEngine/objects/terrain/";

    Texture treeSnow   = Texture( "../GameEngine/textures/T_Tree_winter", "snowTree" );

    // Environment ------------------------------------------------------------------------------

    Texture skyboxBottom = Texture( "../GameEngine/textures/skybox/MusicHall/py.png", "skyboxBottom" );
    Texture skyboxTop    = Texture( "../GameEngine/textures/skybox/MusicHall/ny.png", "skyboxTop"    );
    Texture skyboxRight  = Texture( "../GameEngine/textures/skybox/MusicHall/px.png", "skyboxRight"  );
    Texture skyboxLeft   = Texture( "../GameEngine/textures/skybox/MusicHall/nx.png", "skyboxLeft"   );
    Texture skyboxFront  = Texture( "../GameEngine/textures/skybox/MusicHall/nz.png", "skyboxFront"  );
    Texture skyboxBack   = Texture( "../GameEngine/textures/skybox/MusicHall/pz.png", "skyboxBack"   );

    std::vector<Texture> skyboxTextures = { skyboxRight, skyboxBottom, skyboxFront, skyboxLeft, skyboxTop, skyboxBack };
    cubemap = CubeMap( 15*64, skyboxShader, skyboxTextures );
    renderingEngine.setSkybox( cubemap );

    // Terrain Game Object ------------------------------------------------------------------------------

    Texture heightMap = Texture( "../GameEngine/textures/heightmapTerrain.png", "heightMap" );


    Texture snow    = Texture( "../GameEngine/textures/snowTexture.png", "tex0" );
    Texture rock    = Texture( "../GameEngine/textures/rockTexture.png", "tex1" );
    Texture grass   = Texture( "../GameEngine/textures/grassTexture.png", "tex2" );
    Texture plateau = Texture( "../GameEngine/textures/plateauTexture.png", "tex3" );
    Texture sand    = Texture( "../GameEngine/textures/sandTexture.png", "tex4" );
    Texture hill    = Texture( "../GameEngine/textures/hillTexture.png", "tex5" );

    Texture blendMapTop = Texture( "../GameEngine/textures/blendmapTop.png", "blendmapTop" );
    Texture blendMapMiddle = Texture( "../GameEngine/textures/blendmapMiddle.png", "blendmapMiddle" );
    Texture blendMapBottom = Texture( "../GameEngine/textures/blendmapBottom.png", "blendmapBottom" );

    std::vector<Texture> terrainTextures;

    terrainTextures.push_back( snow );
    terrainTextures.push_back( rock );
    terrainTextures.push_back( grass );
    terrainTextures.push_back( plateau );
    terrainTextures.push_back( sand );
    terrainTextures.push_back( hill );

    terrainTextures.push_back( blendMapTop );
    terrainTextures.push_back( blendMapMiddle );
    terrainTextures.push_back( blendMapBottom );

    terrain = Terrain( 128, 15.0, terrainOBJ, heightMap );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white, false );
    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, this );
    ColliderComponent * terrainCollider = new ColliderComponent( this );

    terrainGO = new GameObjectMesh( "Terrain", terrainRenderer, terrainCollider, false );
    terrainGO->scale( terrain.getScale() );
    terrainGO->move( QVector3D( 64.0 * terrain.getScale(), 0.0, -64.0 * terrain.getScale() ) );

    this->goMeshes.push_back( terrainGO );
    colliderEngine.setTerrain( terrain );

    // Player Game Object  ------------------------------------------------------------------------------

    Texture sandP      = Texture( "../GameEngine/textures/sandTexture.png", "tex0" );
    Texture grassP      = Texture( "../GameEngine/textures/snowrocks.png", "tex1" );

    std::vector<Texture> playerTextures;
    playerTextures.push_back( sandP );
    playerTextures.push_back( grassP );

    Mesh playerMesh = Mesh( bunnyObj, playerTextures, shader, white, false );

    MeshRenderer * playerRenderer      = new MeshRenderer( playerMesh, this );
    MoveComponent * playerMove         = new MoveComponent( terrain, this );
    ColliderComponent * playerCollider = new ColliderComponent( this );
    PlayerComponent * playerComponent  = new PlayerComponent( shader, this );
    PhysicsComponent * playerPhysics   = new PhysicsComponent( physicsEngine, this );

    connect( this, &Game::sendPressedKey, playerMove, &MoveComponent::pressedInput );
    connect( this, &Game::sendReleasedKey, playerMove, &MoveComponent::releasedInput );
    connect( this, &Game::sendMouseMoved, playerMove, &MoveComponent::mouseMoveEvent );

    connect( this, &Game::sendPressedMouse, playerComponent, &PlayerComponent::pressedInput );
    connect( this, &Game::sendreleasedMouse, playerComponent, &PlayerComponent::releasedInput );
    connect( this, &Game::sendMouseWheel, playerComponent, &PlayerComponent::wheelScrolled );

    playerGO  = new GameObjectPlayer( "Player" , playerRenderer, playerMove, playerPhysics, playerCollider, playerComponent );
    playerGO->scale(  QVector3D(0.1, 0.1, 0.1) );
    playerGO->move(  QVector3D(0., 30., -0. ) );


    // Sphere
    std::vector<Texture> sphereTextures;
    sphereTextures.push_back( grass );

    Mesh sphereMesh = Mesh( sphereObj, sphereTextures, shader, white, false );
    MeshRenderer * sphereRenderer = new MeshRenderer( sphereMesh, this  );
    ColliderComponent * sphereCollider = new ColliderComponent( this );
    PhysicsComponent * spherePhysics = new PhysicsComponent(physicsEngine, this );

    sphereGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider,spherePhysics, true, terrainGO );
    sphereGO->scale( 1 );
    sphereGO->move(  QVector3D(0., 200., -0. ));

    this->goMeshes.push_back( sphereGO );

    // Trees  ------------------------------------------------------------------------------

    std::vector<Texture> treeTextures;
    treeTextures.push_back( treeSnow );

    QVector<QVector3D> positionsTree;
    QVector<float> scalesTree;
    QVector<GameObjectMesh*> listTree;
    int numberTree = 10;
    float x = -40.0f;

    for(int i =0 ; i< numberTree; i++){
        positionsTree.push_back(QVector3D(x,0.0f,0.0f));
        x+=8.0f;
    }

    for(int i = 0 ;i<numberTree;i++ ){
        scalesTree.push_back(1);
    }

    for(int i = 0 ;i<numberTree;i++ ){
//        Mesh treeMesh =Mesh( treeObj, treeTextures, shader, white, false );
//        GameObjectMesh * treeGO = new GameObjectMesh( "Tree" + std::to_string(i), new MeshRenderer( treeMesh, this  ), new ColliderComponent( this ), false, terrainGO );
//        treeGO->scale( scalesTree[i] );
//        treeGO->move( positionsTree[i] );
//        listTree.push_back(treeGO);
//        this->goMeshes.push_back( treeGO );
    }

    // Camera  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D();
    QVector3D cameraTarget   = playerGO->getWorldPosition();

    camera = new CameraComponent( cameraPosition, cameraTarget, fov, zNear, zFar );
    MoveComponent * cameraMove = new MoveComponent( terrain, this );
    PhysicsComponent * cameraPhysics = new PhysicsComponent( physicsEngine, this );
    ColliderComponent * cameraCollider = new ColliderComponent( this );

    connect( this, &Game::sendMouseMoved, cameraMove, &MoveComponent::mouseMoveEvent );

    mainCameraGO = new GameObjectCamera("Main camera",camera,cameraMove,cameraPhysics,cameraCollider, playerGO  );
    mainCameraGO->move(0.0f,0.0f,2.0f);
    mainCameraGO->updateCameraPosition();

    renderingEngine.setMainCamera( mainCameraGO );

    // Build hierarchy ---------------------------------------------------------------------------------
    std::vector<GameObject *> baseGo = { sphereGO, playerGO };

    sceneGraph = SceneGraph( baseGo, this->goMeshes, playerGO, mainCameraGO, this->physicsEngine, this->colliderEngine );
}

void Game::input( QKeyEvent * key  ){
    this->sceneGraph.input( key );
}

void Game::update( float fixedStep )
{
    this->sceneGraph.update( fixedStep  );
}

void Game::render(  float deltaTime ){
    renderingEngine.renderScene( this->sceneGraph, deltaTime );
}

// SLOTS
void Game::keyPressed( QKeyEvent * key ){
    emit this->sendPressedKey( key ) ;
}

void Game::keyReleased( QKeyEvent * key ){
    if( key->isAutoRepeat() )
        key->ignore();
    else
        emit this->sendReleasedKey( key ) ;
}

void Game::pressedMouse( QMouseEvent * event ){
    emit this->sendPressedMouse( event );
}

void Game::releasedMouse( QMouseEvent * event ){

    emit this->sendreleasedMouse( event );

}

void Game::mouseWheel( QWheelEvent * event ){
    emit this->sendMouseWheel( event );

}

void Game::mouseMoved( QVector2D pos, bool reset ){
    emit this->sendMouseMoved(pos, reset )  ;
}
// Getters & Setters

CameraComponent *Game::getCamera() const{
    return camera;
}

void Game::setCamera(CameraComponent *newCamera){
    camera = newCamera;
}

const PhysicsEngine &Game::getPhysicsEngine() const{
    return physicsEngine;
}

void Game::setPhysicsEngine(const PhysicsEngine &newPhysicsEngine){

    physicsEngine = newPhysicsEngine;

}

void Game::setProjection( float aspect ){
    this->camera->setProjectionPersp(aspect );
}

const ColliderEngine &Game::getColliderEngine() const
{
    return colliderEngine;
}

void Game::setColliderEngine(const ColliderEngine &newColliderEngine)
{
    colliderEngine = newColliderEngine;
}

const RenderingEngine &Game::getRenderingEngine() const
{
    return renderingEngine;
}

void Game::setRenderingEngine(const RenderingEngine &newRenderingEngine)
{
    renderingEngine = newRenderingEngine;
}
