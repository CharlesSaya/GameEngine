#ifndef GAME_H
#define GAME_H

#include <vector>

#include <QOpenGLShaderProgram>

#include "headers/core/player.h"
#include "headers/core/camera.h"

#include "headers/core/gameObject.h"
#include "headers/core/gameObjectMesh.h"
#include "headers/core/gameObjectPlayer.h"
#include "headers/core/gameObjectCamera.h"

#include "headers/core/meshrenderer.h"
#include "headers/core/moveComponent.h"
#include "headers/core/gameComponent.h"
#include "headers/core/physicsComponent.h"

#include "headers/render/light.h"
#include "headers/render/shader.h"
#include "headers/render/terrain.h"

#include "sceneGraph.h"

class Game : public QObject{
    Q_OBJECT

signals:
    void sendPressedKey( QKeyEvent * event );
    void sendreleasedKey( QKeyEvent * event );

public slots:
    void keyPressed( QKeyEvent * key );
    void keyReleased( QKeyEvent * key );

public:
    Game(   QObject * parent = 0  );
    Game( Camera * camera, QObject * parent = 0 );
    void initGame();

    void input( QKeyEvent * key );
    void update( float deltaTimetime );
    void render();

    void updatePosition();
    void updateCollision();

    Player &getPlayer();
    void setPlayer(const Player &newPlayer);

    Camera *getCamera() const;
    void setCamera(Camera *newCamera);

    const PhysicsEngine &getPhysicsEngine() const;
    void setPhysicsEngine(const PhysicsEngine &newPhysicsEngine);

    void setProjection( float aspect );

private:
    float deltaTime;
    Camera * camera;
    SceneGraph sceneGraph;
    Player player;

    Terrain terrain;

    GameObjectMesh * terrainGO ;
    GameObjectMesh * sphereGO ;
    GameObjectPlayer * playerGO;

    Shader * shader ;
    Shader * terrainShader;

    PhysicsEngine physicsEngine;

    QVector3D white = QVector3D( 1., 0., 0.);

    std::vector< GameObjectMesh *> goMeshes;
    std::vector< GameObjectPlayer *> goPlayers;
    std::vector< GameObjectCamera *> goCameras;
};

#endif // GAME_H
