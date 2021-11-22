#ifndef GAME_H
#define GAME_H

#include <vector>

#include <QOpenGLShaderProgram>

#include "headers/core/player.h"
#include "headers/core/camera.h"
#include "headers/core/gameObject.h"
#include "headers/core/meshrenderer.h"
#include "headers/core/moveComponent.h"
#include "headers/core/gameComponent.h"

#include "headers/render/light.h"
#include "headers/render/shader.h"
#include "headers/render/terrain.h"

#include "sceneGraph.h"

class Game : public QObject{
    Q_OBJECT

public slots:

    void setProjection( float aspect );

public:
    Game(  );
    void initGame();

    void input( float time );
    void update( float time );
    void render();

    Camera &getCamera();
    void setCamera(const Camera &newCamera);

    Player &getPlayer();
    void setPlayer(const Player &newPlayer);

private:
    float deltaTime;

    SceneGraph sceneGraph;
    Player player;

    Terrain terrain;
    Camera * camera;

    GameObject * terrainGO ;
    GameObject * playerGO;

    Shader * shader ;
    Shader * terrainShader;

    QVector3D white = QVector3D( 1., 0., 0.);
};

#endif // GAME_H
