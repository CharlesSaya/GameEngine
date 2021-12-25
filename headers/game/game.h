#ifndef GAME_H
#define GAME_H

#include <vector>

#include <QOpenGLShaderProgram>

#include "headers/core/gameObject.h"
#include "headers/core/gameObjectMesh.h"
#include "headers/core/gameObjectPlayer.h"
#include "headers/core/gameObjectCamera.h"

#include "headers/core/meshrenderer.h"
#include "headers/core/moveComponent.h"
#include "headers/core/gameComponent.h"
#include "headers/core/cameraComponent.h"
#include "headers/core/playerComponent.h"
#include "headers/core/physicsComponent.h"
#include "headers/core/colliderComponent.h"

#include "headers/physics/physicsEngine.h"
#include "headers/physics/colliderEngine.h"

#include "headers/render/light.h"
#include "headers/render/shader.h"
#include "headers/render/terrain.h"
#include "headers/render/cubemap.h"

#include "sceneGraph.h"

class Game : public QObject{
    Q_OBJECT

signals:
    void sendPressedKey( QKeyEvent * event );
    void sendReleasedKey( QKeyEvent * event );
    void sendMouseMoved( QMouseEvent * event );

    void sendPressedMouse( QMouseEvent * event );
    void sendreleasedMouse( QMouseEvent * event );

    void sendMouseWheel( QWheelEvent * event );

public slots:
    void keyPressed( QKeyEvent * key );
    void keyReleased( QKeyEvent * key );
    void mouseMoved( QMouseEvent * key );

    void pressedMouse( QMouseEvent * event );
    void releasedMouse( QMouseEvent * event );

    void mouseWheel( QWheelEvent * event );


public:
    Game(   QObject * parent = 0  );
    Game( CameraComponent * camera, QObject * parent = 0 );
    void initGame();

    void input( QKeyEvent * key );
    void update( float deltaTimetime );
    void render();

    void updatePosition();
    void updateCollision();

    CameraComponent *getCamera() const;
    void setCamera(CameraComponent *newCamera);

    const PhysicsEngine &getPhysicsEngine() const;
    void setPhysicsEngine(const PhysicsEngine &newPhysicsEngine);

    void setProjection( float aspect );

    const ColliderEngine &getColliderEngine() const;
    void setColliderEngine(const ColliderEngine &newColliderEngine);

private:
    float deltaTime;
    CameraComponent * camera;
    SceneGraph sceneGraph;

    Terrain terrain;

    GameObjectMesh * terrainGO ;
    GameObjectMesh * sphereGO ;
    GameObjectPlayer * playerGO;
    GameObjectCamera *mainCameraGO ;

    Shader * shader ;
    Shader * terrainShader;
    Shader * skyboxShader;

    CubeMap cubemap;

    PhysicsEngine physicsEngine;
    ColliderEngine colliderEngine;

    QVector3D white = QVector3D( 1., 0., 0.);

    std::vector< GameObjectMesh *> goMeshes;
    std::vector< GameObjectPlayer *> goPlayers;
    std::vector< GameObjectCamera *> goCameras;
};

#endif // GAME_H
