#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "headers/core/camera.h"

#include "headers/core/gameObject.h"
#include "headers/core/gameObjectMesh.h"
#include "headers/core/gameObjectPlayer.h"
#include "headers/core/gameObjectCamera.h"

#include "headers/render/shader.h"
#include "headers/render/renderingEngine.h"

#include "headers/physics/physicsEngine.h"

typedef struct Node{

    Node * parent = 0;
    GameObject * gameObject = 0;
    AABB * nodeBoundingBox = 0;

    std::vector<Node *> children;

    Node(){
        this->nodeBoundingBox = new AABB();
    }

} Node;


class SceneGraph{

private:
    Node * root;
    std::vector<GameObject *> goList;
    std::vector<GameObject *> goCollidable;
    std::vector<GameObjectMesh *> goMeshes;
    std::vector<GameObjectPlayer *> goPlayers;
    std::vector<GameObjectCamera *> goCameras;


public:

    SceneGraph();
    SceneGraph( std::vector<GameObject *>& goList, std::vector<GameObjectMesh *> &goMeshes, std::vector<GameObjectPlayer *> &goPlayers, std::vector<GameObjectCamera *> &goCameras );

    Node * getRoot();

    Node * buildGraphScene( GameObject * go );
    void transformBBox( Node * node );
    bool isLeaf( Node * node);

    void input ( QKeyEvent * key );
    void update( float fixedStep );
    void render( Camera &camera  );

    template<class Physics>
    void updatePhysics( Physics * go, float step ){
        go->getPhysicsComponent()->updatePhysics( step, *go->getTransform() );
    }

    template<class Renderable>
    void renderMesh( Renderable * go, Camera & camera ){
        go->getMeshRenderer()->renderMesh( *go->getTransform(), go->getModel(), camera );
    }

    template<class Movable>
    void updateCollision( Movable * go, Camera & camera ){
        go->getCollisionComponent()->computeCollision();
    }

    void updateBVH( Node * node );

};

#endif // SCENEGRAPH_H
