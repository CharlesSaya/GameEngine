#include "headers/game/game.h"

Game::Game( QObject * parent ) : QObject(parent){

}

Game::Game( Camera * camera,  QObject * parent ) : QObject(parent){
    this->camera = camera;
}

void Game::initGame(){

    // Shaders & Lights  --------------------------------------------------------------------------------

    shader = new Shader( "../GameEngine/shaders/base_vshader.glsl", "../GameEngine/shaders/base_fshader.glsl" );
    terrainShader = new Shader(  "../GameEngine/shaders/terrain_vshader.vert", "../GameEngine/shaders/terrain_fshader.frag" );

    Light light( QVector3D( 5.0, 4.0, -5.0) );
    light.loadLight( shader );
    light.loadLight( terrainShader );

    // Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere/";
    std::string bunnyObj  = "../GameEngine/objects/bunny/";

    Texture heightMap = Texture( "../GameEngine/textures/Heightmap_Rocky.png", "heightMap" );
    Texture snow      = Texture( "../GameEngine/textures/snowrocks.png", "snow" );
    Texture rock      = Texture( "../GameEngine/textures/rock.png", "rock" );
    Texture grass     = Texture( "../GameEngine/textures/grass.png", "grass" );

    // Terrain Game Object ------------------------------------------------------------------------------

    std::vector<Texture> terrainTextures;
    terrainTextures.push_back( heightMap );
    terrainTextures.push_back( snow );
    terrainTextures.push_back( rock );
    terrainTextures.push_back( grass );

    terrain = Terrain( heightMap );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white );

    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh );
    ColliderComponent * terrainCollider = new ColliderComponent();

    terrainGO = new GameObjectMesh( "Terrain", terrainRenderer, terrainCollider );

    this->goMeshes.push_back( terrainGO );

    // Player Game Object  ------------------------------------------------------------------------------

    std::vector<Texture> playerTextures;
    playerTextures.push_back( grass );

    Mesh playerMesh = Mesh( bunnyObj ,playerTextures, shader, white );   

    MeshRenderer * playerRenderer = new MeshRenderer( playerMesh );
    PhysicsComponent * playerPhysics = new PhysicsComponent( physicsEngine, this );

    MoveComponent * playerMove = new MoveComponent( terrain );
    ColliderComponent * playerCollider = new ColliderComponent();

    connect( this, &Game::sendPressedKey, playerMove, &MoveComponent::pressedInput );
    connect( this, &Game::sendreleasedKey, playerMove, &MoveComponent::releasedInput );

    playerGO  = new GameObjectPlayer( "Player" , playerRenderer, playerMove, playerPhysics, playerCollider );
    playerGO->scale( QVector3D( 0.01, 0.01, 0.01 ) );

    this->player = Player( *playerGO );
    this->player.setMesh( playerMesh );
    this->player.move( QVector3D(0.0, 0.0, 0.0), terrain ); // set initial height

    this->goPlayers.push_back( playerGO );

    // Sphere

    std::vector<Texture> sphereTextures;
    playerTextures.push_back( grass );

    Mesh sphereMesh = Mesh( sphereObj, sphereTextures, shader, white );
    MeshRenderer * sphereRenderer = new MeshRenderer( sphereMesh );
    ColliderComponent * sphereCollider = new ColliderComponent();

    sphereGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider, playerGO );
    sphereGO->move( QVector3D(0.0, 1.0, 0.0)  );

    this->goMeshes.push_back( sphereGO );

    // Build hierarchy ---------------------------------------------------------------------------------

    std::vector<GameObject *> baseGo = { terrainGO, playerGO };

    sceneGraph = SceneGraph( baseGo, this->goMeshes, this->goPlayers, this->goCameras );

}

void Game::input( QKeyEvent * key  ){
    this->sceneGraph.input( key );

}

void Game::update( float fixedStep )
{
    this->sceneGraph.update( fixedStep  );
}

void Game::render( ){
    this->sceneGraph.render( *camera );
}


// SLOTS
void Game::keyPressed( QKeyEvent * key ){

    emit( this->sendPressedKey( key ) );
}

void Game::keyReleased( QKeyEvent * key ){
    emit( this->sendreleasedKey( key ) );
}

// Getters & Setters

Camera *Game::getCamera() const{
    return camera;
}


void Game::setCamera(Camera *newCamera){
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
