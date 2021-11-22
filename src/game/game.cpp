#include "headers/game/game.h"

Game::Game( ){

}

void Game::initGame(){

    // Shaders & Lights  -------------------------------------------------------------------------------

    shader = new Shader( "../GameEngine/shaders/base_vshader.glsl", "../GameEngine/shaders/base_fshader.glsl" );
    terrainShader = new Shader(  "../GameEngine/shaders/terrain_vshader.vert", "../GameEngine/shaders/terrain_fshader.frag" );

    Light light( QVector3D( 5.0, 4.0, -5.0) );
    light.loadLight( shader );
    light.loadLight( terrainShader );

    // Cameras  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D(2.0,2.0,1.);
    QVector3D cameraTarget   = QVector3D(.0,.0,.0);

    const qreal zNear = .01, zFar = 100.0, fov = 80.0;

    camera = new Camera( cameraPosition, cameraTarget, fov, zNear, zFar );

    // Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere/";
    std::string bunnyObj  = "../GameEngine/objects/bunny/";

    Texture heightMap = Texture( "../GameEngine/textures/Heightmap_Rocky.png", "heightMap" );
    Texture snow      = Texture( "../GameEngine/textures/snowrocks.png", "snow" );
    Texture rock      = Texture( "../GameEngine/textures/rock.png", "rock" );
    Texture grass     = Texture( "../GameEngine/textures/grass.png", "grass" );

    // Terrain Game Object --------------------------------------------------------------------------

    std::vector<Texture> terrainTextures;
    terrainTextures.push_back( heightMap );
    terrainTextures.push_back( snow );
    terrainTextures.push_back( rock );
    terrainTextures.push_back( grass );

    terrain = Terrain( heightMap );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white );

    terrainGO = new GameObject( "Terrain" );

    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, terrainGO->getTransform() );

    terrainGO->addComponent( terrainRenderer );

    // Player Game Object  -------------------------------------------------------------------------------


    std::vector<Texture> bunnyTextures;
    bunnyTextures.push_back( grass );

    Mesh playerMesh = Mesh( sphereObj ,bunnyTextures, shader, white );

    playerGO  = new GameObject( "Player", terrainGO );
    playerGO->scale( QVector3D( 0.01, 0.01, 0.01 ) );

    MeshRenderer * playerRenderer = new MeshRenderer( playerMesh, playerGO->getTransform() );

    MoveComponent * playerMove = new MoveComponent( deltaTime, playerGO->getTransform(), terrain );

    installEventFilter( playerMove );

    playerGO->addComponent( playerRenderer );

    this->player = Player( *playerGO );
    this->player.setMesh( playerMesh );
    this->player.move( QVector3D(0.0, 0.0, 0.0), terrain );

    // Build hierarchy --------------------------------------------------------------------------

    terrainGO->addChild( playerGO );

    sceneGraph = SceneGraph( terrainGO );
}

void Game::input( float time ){

}
void Game::update( float time )
{

}

void Game::render( ){
    this->sceneGraph.render( terrainGO, *camera );
}

void Game::setProjection( float aspect ){
    this->camera->setProjection( aspect );
}
