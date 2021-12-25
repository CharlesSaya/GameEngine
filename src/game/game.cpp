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

    Light light( QVector3D( 5.0, 4.0, -5.0) );
    light.loadLight( shader );
    light.loadLight( terrainShader );
    light.loadLight( skyboxShader );

    // Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere/";
    std::string bunnyObj  = "../GameEngine/objects/bunny/";

    Texture heightMap = Texture( "../GameEngine/textures/Heightmap_Rocky.png", "heightMap" );
    Texture snow      = Texture( "../GameEngine/textures/snowrocks.png", "snow" );
    Texture rock      = Texture( "../GameEngine/textures/rock.png", "rock" );
    Texture grass     = Texture( "../GameEngine/textures/grass.png", "grass" );

    Texture skyboxBottom = Texture( "../GameEngine/textures/skybox/MusicHall/py.png", "skyboxBottom" );
    Texture skyboxTop    = Texture( "../GameEngine/textures/skybox/MusicHall/ny.png", "skyboxTop" );
    Texture skyboxRight  = Texture( "../GameEngine/textures/skybox/MusicHall/px.png", "skyboxRight" );
    Texture skyboxLeft   = Texture( "../GameEngine/textures/skybox/MusicHall/nx.png", "skyboxLeft" );
    Texture skyboxFront  = Texture( "../GameEngine/textures/skybox/MusicHall/nz.png", "skyboxFront" );
    Texture skyboxBack   = Texture( "../GameEngine/textures/skybox/MusicHall/pz.png", "skyboxBack" );

    // Environment

    std::vector<Texture> skyboxTextures = { skyboxRight, skyboxBottom, skyboxFront, skyboxLeft, skyboxTop, skyboxBack };
    cubemap = CubeMap( 50, skyboxShader, skyboxTextures );

    // Terrain Game Object ------------------------------------------------------------------------------

    std::vector<Texture> terrainTextures;
    terrainTextures.push_back( heightMap );
    terrainTextures.push_back( snow );
    terrainTextures.push_back( rock );
    terrainTextures.push_back( grass );

    terrain = Terrain( heightMap );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white, true );
    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, this );
    ColliderComponent * terrainCollider = new ColliderComponent( this );

    terrainGO = new GameObjectMesh( "Terrain", terrainRenderer, terrainCollider );
    this->goMeshes.push_back( terrainGO );



    // Player Game Object  ------------------------------------------------------------------------------

    std::vector<Texture> playerTextures;
    playerTextures.push_back( grass );

    Mesh playerMesh = Mesh( bunnyObj ,playerTextures, shader, white, true );

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
    playerGO->scale( QVector3D(0.1, 0.1, 0.1) );
    playerGO->move(  QVector3D(2., 2., -5.) );

    this->goPlayers.push_back( playerGO );

    // Sphere
    std::vector<Texture> sphereTextures;
    playerTextures.push_back( grass );

    Mesh sphereMesh = Mesh( sphereObj, sphereTextures, shader, white, true );
    MeshRenderer * sphereRenderer = new MeshRenderer( sphereMesh, this  );
    ColliderComponent * sphereCollider = new ColliderComponent( this );

    sphereGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider, playerGO );
    sphereGO->scale( QVector3D(0.1, 0.1, 0.1) );

    this->goMeshes.push_back( sphereGO );

    // Camera  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D();
    QVector3D cameraTarget   = playerGO->getWorldPosition();
    const qreal zNear = .01, zFar = 100.0, fov = 80.0;
    camera = new CameraComponent( cameraPosition, cameraTarget, fov, zNear, zFar );

    MoveComponent * cameraMove = new MoveComponent( terrain, this );
    PhysicsComponent * cameraPhysics = new PhysicsComponent( physicsEngine, this );
    ColliderComponent * cameraCollider = new ColliderComponent( this );

    mainCameraGO = new GameObjectCamera("Main camera",camera,cameraMove,cameraPhysics,cameraCollider, playerGO  );

    mainCameraGO->updateCameraPosition();
    this->goCameras.push_back(mainCameraGO);

    // Build hierarchy ---------------------------------------------------------------------------------
//    this->playerGO->addChild( sphereGO );
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

void Game::render( ){
    cubemap.render( mainCameraGO, QMatrix4x4() );
    this->sceneGraph.render( *camera );
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

void Game::mouseMoved( QMouseEvent * key ){
    emit this->sendMouseMoved(key)  ;
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
    this->camera->setProjection( aspect );
}

const ColliderEngine &Game::getColliderEngine() const
{
    return colliderEngine;
}

void Game::setColliderEngine(const ColliderEngine &newColliderEngine)
{
    colliderEngine = newColliderEngine;
}
