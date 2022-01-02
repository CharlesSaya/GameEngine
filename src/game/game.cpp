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

    Texture heightMap = Texture( "../GameEngine/textures/desert.png", "heightMap" );
    Texture snow      = Texture( "../GameEngine/textures/snowrocks.png", "texture2" );
    Texture rock      = Texture( "../GameEngine/textures/rock.png", "texture1" );
    Texture rock2      = Texture( "../GameEngine/textures/rock.png", "texture0" );
    Texture grass     = Texture( "../GameEngine/textures/grass.png", "texture0" );

    Texture skyboxBottom = Texture( "../GameEngine/textures/skybox/MusicHall/py.png", "skyboxBottom" );
    Texture skyboxTop    = Texture( "../GameEngine/textures/skybox/MusicHall/ny.png", "skyboxTop"    );
    Texture skyboxRight  = Texture( "../GameEngine/textures/skybox/MusicHall/px.png", "skyboxRight"  );
    Texture skyboxLeft   = Texture( "../GameEngine/textures/skybox/MusicHall/nx.png", "skyboxLeft"   );
    Texture skyboxFront  = Texture( "../GameEngine/textures/skybox/MusicHall/nz.png", "skyboxFront"  );
    Texture skyboxBack   = Texture( "../GameEngine/textures/skybox/MusicHall/pz.png", "skyboxBack"   );

    Texture treeSnow   = Texture( "../GameEngine/textures/T_Tree_winter", "snowTree" );

    // Environment

    std::vector<Texture> skyboxTextures = { skyboxRight, skyboxBottom, skyboxFront, skyboxLeft, skyboxTop, skyboxBack };
    cubemap = CubeMap( 50, skyboxShader, skyboxTextures );
    renderingEngine.setSkybox( cubemap );

    // Terrain Game Object ------------------------------------------------------------------------------

    std::vector<Texture> terrainTextures;
    terrainTextures.push_back( heightMap );

    terrain = Terrain( heightMap, 1.0f );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white, false );
    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, this );
    ColliderComponent * terrainCollider = new ColliderComponent( this );

    terrainGO = new GameObjectMesh( "Terrain", terrainRenderer, terrainCollider, false );
    this->goMeshes.push_back( terrainGO );
    colliderEngine.setTerrain( terrain );
    // Player Game Object  ------------------------------------------------------------------------------

    std::vector<Texture> playerTextures;
    playerTextures.push_back( rock2 );

    Mesh playerMesh = Mesh( bunnyObj ,playerTextures, shader, white, false );

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
    playerGO->move(  QVector3D(0., 30., -0. ) );

    this->goPlayers.push_back( playerGO );

    // Sphere
    std::vector<Texture> sphereTextures;
    sphereTextures.push_back( grass );

    Mesh sphereMesh = Mesh( sphereObj, sphereTextures, shader, white, false );
    MeshRenderer * sphereRenderer = new MeshRenderer( sphereMesh, this  );
    ColliderComponent * sphereCollider = new ColliderComponent( this );

    sphereGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider, true, terrainGO );
    sphereGO->scale( 1 );
    sphereGO->move( QVector3D(2., 2., -0.) );

    this->goMeshes.push_back( sphereGO );

    //Tree
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
        Mesh treeMesh =Mesh( treeObj, treeTextures, shader, white, false );
        GameObjectMesh * treeGO = new GameObjectMesh( "Tree" + std::to_string(i), new MeshRenderer( treeMesh, this  ), new ColliderComponent( this ), false, terrainGO );
        treeGO->scale( scalesTree[i] );
        treeGO->move( positionsTree[i] );
        listTree.push_back(treeGO);
        this->goMeshes.push_back( treeGO );
    }

    // Light
//    sphereTextures.push_back( grass );
//    sphereLightGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider, terrainGO );
//    sphereLightGO->move( renderingEngine.getLight().getLightPosition() );
//    sphereLightGO->move(  QVector3D(0., -4., 0.) );

//    this->goMeshes.push_back( sphereLightGO );

    // Camera  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D();
    QVector3D cameraTarget   = playerGO->getWorldPosition();
    const qreal zNear = .01, zFar = 1000.0, fov = 80.0;
    camera = new CameraComponent( cameraPosition, cameraTarget, fov, zNear, zFar );
    MoveComponent * cameraMove = new MoveComponent( terrain, this );
    PhysicsComponent * cameraPhysics = new PhysicsComponent( physicsEngine, this );
    ColliderComponent * cameraCollider = new ColliderComponent( this );
    connect( this, &Game::sendMouseMoved, cameraMove, &MoveComponent::mouseMoveEvent );
    mainCameraGO = new GameObjectCamera("Main camera",camera,cameraMove,cameraPhysics,cameraCollider, playerGO  );
    mainCameraGO->move(0.0f,0.0f,0.0f);
    mainCameraGO->updateCameraPosition();
    this->goCameras.push_back(mainCameraGO);
    renderingEngine.setMainCamera( mainCameraGO );

    // Build hierarchy ---------------------------------------------------------------------------------
    std::vector<GameObject *> baseGo = { terrainGO, playerGO };

    sceneGraph = SceneGraph( baseGo, this->goMeshes, this->goPlayers, this->goCameras, this->physicsEngine, this->colliderEngine );
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
