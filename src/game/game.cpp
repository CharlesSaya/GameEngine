#include "headers/game/game.h"


Game::Game(){
}

void Game::initGame(){

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

Player &Game::getPlayer()
{
    return player;
}

void Game::setPlayer(const Player &newPlayer)
{
    player = newPlayer;
}

