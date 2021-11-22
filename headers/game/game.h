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
    Game();
    Game( Camera * camera );
    void initGame();

    void input( QKeyEvent * key, float deltaTime );
    void update( float deltaTimetime );
    void render();



    Player &getPlayer();
    void setPlayer(const Player &newPlayer);

    Camera *getCamera() const;
    void setCamera(Camera *newCamera);

private:
    float deltaTime;
    Camera * camera;
    SceneGraph sceneGraph;
    Player player;

    Terrain terrain;

    GameObject * terrainGO ;
    GameObject * playerGO;

    Shader * shader ;
    Shader * terrainShader;

    QVector3D white = QVector3D( 1., 0., 0.);
};

#endif // GAME_H
