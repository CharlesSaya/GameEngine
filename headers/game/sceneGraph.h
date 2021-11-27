#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "headers/core/camera.h"
#include "headers/core/gameObject.h"
#include "headers/core/colliderComponent.h"

#include "headers/render/shader.h"
#include "headers/render/vertexData.h"

#include "headers/physics/physicsEngine.h"
#include "headers/render/renderingEngine.h"
#include "headers/render/renderingEngine.h"

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

public:

    SceneGraph();
    SceneGraph( std::vector<GameObject *>& goList );

    Node * getRoot();

    Node * buildGraphScene( GameObject * go );
    void transformBBox( Node * node );
    bool isLeaf( Node * node);

    void input ( QKeyEvent * key );
    void update( float fixedStep );
    void render( Camera &camera  );

    void updateBBox( Node * node );

};

#endif // SCENEGRAPH_H
