#include "headers/game/game.h"

Game::Game(){

}

Game::Game( Camera * camera ){
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

    terrainGO = new GameObject( "Terrain" );

    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, terrainGO->getTransform() );
    ColliderComponent * terrainCollider = new ColliderComponent( terrainMesh );

    terrainGO->addComponent( terrainRenderer );
    terrainGO->addComponent( terrainCollider );

    // Player Game Object  ------------------------------------------------------------------------------

    std::vector<Texture> playerTextures;
    playerTextures.push_back( grass );

    Mesh playerMesh = Mesh( bunnyObj ,playerTextures, shader, white );
    playerGO  = new GameObject( "Player" );
    playerGO->scale( QVector3D( 0.01, 0.01, 0.01 ) );

    MeshRenderer * playerRenderer = new MeshRenderer( playerMesh, playerGO->getTransform() );
    PhysicsComponent * playerPhysics = new PhysicsComponent( playerGO->getTransform(), physicsEngine );
    MoveComponent * playerMove = new MoveComponent( deltaTime, terrain );
    ColliderComponent * playerCollider = new ColliderComponent( playerMesh );

    connect( playerMove,  &MoveComponent::move, playerPhysics, &PhysicsComponent::hasMoved );
    connect( playerMove,  &MoveComponent::stop, playerPhysics, &PhysicsComponent::hasStopped );

    connect( this, &Game::sendPressedKey, playerMove, &MoveComponent::pressedInput );
    connect( this, &Game::sendreleasedKey, playerMove, &MoveComponent::releasedInput );

    playerGO->addComponent( playerRenderer );
    playerGO->addComponent( playerMove );
    playerGO->addComponent( playerPhysics );
    playerGO->addComponent( playerCollider );

    this->player = Player( *playerGO );
    this->player.setMesh( playerMesh );
    this->player.move( QVector3D(0.0, 0.0, 0.0), terrain ); // set initial height

    // Build hierarchy ---------------------------------------------------------------------------------


    std::vector<GameObject *> baseGo = { terrainGO, playerGO };

    sceneGraph = SceneGraph( baseGo );

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
