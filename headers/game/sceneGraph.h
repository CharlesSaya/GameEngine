#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QFileInfo>


#include "headers/game/node.h"


#include "headers/core/gameObject.h"
#include "headers/core/gameObjectMesh.h"
#include "headers/core/gameObjectPlayer.h"
#include "headers/core/gameObjectCamera.h"

#include "headers/render/shader.h"

#include "headers/physics/physicsEngine.h"
#include "headers/physics/colliderEngine.h"

class SceneGraph{

private:
    Node * root;
    std::vector<GameObject *> goList;
    std::vector<GameObject *> goCollidable;
    std::vector<GameObjectMesh *> goMeshes;
    GameObjectPlayer * goPlayer;
    GameObjectCamera * goCamera;
    GameObjectCamera * mainCamera;
    PhysicsEngine physicsEngine;
    ColliderEngine colliderEngine;
    Terrain terrain;
    QElapsedTimer timer;
    int elapsedTime = 0;
    float gridSpeed = -0.000002f;

public:

    SceneGraph();
    SceneGraph( std::vector<GameObject *>& goList,
                std::vector<GameObjectMesh *> &goMeshes,
                GameObjectPlayer * &goPlayer,
                GameObjectCamera * &goCameras,
                PhysicsEngine & physicsEngine,
                ColliderEngine & colliderEngine,
                Terrain & terrain);

    Node * getRoot();
    void checkCollectibleNumber(GameObjectMesh *grid);
    void rotateRing(GameObjectMesh *ring);
    Node * buildGraphScene( GameObject * go );
    bool isLeaf( Node * node);
    void input ( QKeyEvent * key );
    void update( float fixedStep );
    void render( GameObjectCamera * camera, Shader * shader = 0 );
    void renderBVH( Node * node, Shader * shader );

    void destroyGOs( );

    template<class Movable>
    void updatePhysics( Movable * go, float step ){
        go->getPhysicsComponent()->updatePhysics( step, go, terrain );
    }

    template<class Movable>
    void updatePhysicsMesh( Movable * go, float step ){
        go->getPhysicsComponent()->updatePhysicsMesh( step, go );
    }

    template<class Renderable>
    void renderMesh( Renderable * go,  GameObjectCamera * camera, Shader * shader ){
         go->getMeshRenderer()->renderMesh( *go->getTransform(), go->getModel(), camera->getCameraComponent(), shader );
    }

    template<class Collidable>
    void computeCollision( Collidable * go ){
        colliderEngine.detectCollision( go, this->root );
    }

    template<class Collidable>
    void checkRestingCondition( Collidable * go ){

    }

    void updateBVH( Node * node );
    void updateALLBVH();
    void rayBVHCollision(  Node * node );
    const std::vector<GameObjectMesh *> &getGoMeshes() const;
    GameObjectPlayer *getGoPlayer() const;
};

#endif // SCENEGRAPH_H
