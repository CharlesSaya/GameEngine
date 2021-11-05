#ifndef GAME_H
#define GAME_H

#include <vector>

#include <QOpenGLShaderProgram>

#include "../core/gameObject.h"
#include "../core/meshrenderer.h"
#include "../core/gameComponent.h"

#include "../render/camera.h"
#include "../render/BasicShader.h"

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

private:

    Camera camera;
    SceneGraph sceneGraph;

    GameObject * sun ;
    GameObject * moon ;
    GameObject * earth ;

    GameObject * solarSystem ;
    GameObject * moonOrbit ;
    GameObject * earthOrbit ;

;
};

#endif // GAME_H
