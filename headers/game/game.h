#ifndef GAME_H
#define GAME_H

#include <vector>

#include <QOpenGLShaderProgram>

#include "headers/core/player.h"
#include "headers/core/camera.h"
#include "headers/core/gameObject.h"
#include "headers/core/meshrenderer.h"
#include "headers/core/gameComponent.h"

#include "headers/render/shader.h"
#include "headers/render/terrain.h"

#include "sceneGraph.h"

class Game{

public:
    Game();
    void initGame();
    void initTextures();

    void input( float time );
    void update( float time );
    void render( QMatrix4x4 projection );

    Camera &getCamera();
    void setCamera(const Camera &newCamera);

    Player &getPlayer();
    void setPlayer(const Player &newPlayer);

private:

    Camera camera;
    SceneGraph sceneGraph;
    Player player;

    GameObject * sun ;
    GameObject * moon ;
    GameObject * earth ;
    GameObject * plane ;

    GameObject * solarSystem ;
    GameObject * moonOrbit ;
    GameObject * earthOrbit ;

};

#endif // GAME_H
