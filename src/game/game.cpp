#include "headers/game/game.h"


Game::Game(){
}

void Game::initGame(){
    BasicShader& shader = BasicShader::getInstance();

    shader.useShaderProgram();

    // Cameras  -------------------------------------------------------------------------------
    QVector3D cameraPosition = QVector3D(.0,0.0,4.);
    QVector3D cameraTarget = QVector3D(.0,.0,0.);

    camera = Camera( cameraPosition, cameraTarget );

    //Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere.obj";
    std::string bunnyObj  = "../GameEngine/objects/bunny.obj";

    solarSystem = new GameObject( "Solar System", nullptr ) ;
    sun   = new GameObject( "Sun", sphereObj, QVector3D( 1., 1., 0. ), solarSystem);
    MeshRenderer * sunRenderer = new MeshRenderer( &shader.getProgram(), sun->getMesh() );
    sun->addComponent( sunRenderer );

    earthOrbit  = new GameObject( "Earth Orbit", solarSystem ) ;
    earth = new GameObject( "Earth", sphereObj, QVector3D( 0., 0., 1. ), earthOrbit );
    MeshRenderer * earthRenderer = new MeshRenderer( &shader.getProgram(), earth->getMesh() );
    earth->addComponent( earthRenderer );

    moonOrbit   = new GameObject( " Moon Orbit ", earthOrbit ) ;
    moon  = new GameObject( "Moon", sphereObj, QVector3D( 0.75, 0.75, 0.75 ), moonOrbit );
    MeshRenderer * moonRenderer = new MeshRenderer( &shader.getProgram(), moon->getMesh() );
    moon->addComponent( moonRenderer );

    solarSystem->addChild( sun );
    solarSystem->addChild( earthOrbit );

    earthOrbit->addChild( earth );
    earthOrbit->addChild( moonOrbit );

    moonOrbit->addChild( moon );

    sceneGraph = SceneGraph( solarSystem );
}

void Game::input( float time ){

}

void Game::update( float time ){

    sun->resetModelMatrix();
    sun->rotate( QVector3D(0.,1.,0.), - 50. * time  );

    earthOrbit->resetModelMatrix();
    earthOrbit->rotate( QVector3D( 0.,1.,0. ), -20 * time );
    earthOrbit->move( QVector3D( 3., 0.0, 0.0 ) );
    earthOrbit->rotate( QVector3D( 0.,0.,1. ), -23.44  );

    earth->resetModelMatrix();
    earth->scale( QVector3D( .5, .5, .5 ) );
    earth->rotate( QVector3D(0.,1.,0.), -20 * time );

    moonOrbit->resetModelMatrix();
    moonOrbit->rotate( QVector3D(0.,1.,0.), 10 * -20 * time );
    moonOrbit->move(QVector3D( 1.0, 0.0, 0.0 ));

    moon->resetModelMatrix();
    moon->scale( QVector3D( .15, .15, .15) );
    moon->rotate( QVector3D(0.,1.,0.), -10 * 20 * time );

}

void Game::render( QMatrix4x4 projection ){
    BasicShader& shader = BasicShader::getInstance();
    this->sceneGraph.render( sceneGraph.getRoot(), camera.getViewMatrix() , projection, &shader.getProgram() );

}


void Game::initTextures()
{
    std::string textures[4] = { ":/textures/rock.png",
                               ":/textures/grass.png" ,
                               ":/textures/snowrocks.png",
                               ":/textures/heightmap-1024x1024.png" };
}

Camera &Game::getCamera()
{
    return camera;
}

void Game::setCamera(const Camera &newCamera)
{
    camera = newCamera;
}

