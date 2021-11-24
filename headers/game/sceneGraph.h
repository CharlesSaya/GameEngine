#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "headers/core/camera.h"
#include "headers/core/gameObject.h"
#include "headers/render/shader.h"
#include "headers/physics/physicsEngine.h"
#include "headers/render/renderingEngine.h"

class SceneGraph{
private:
    GameObject * root;

public:

    SceneGraph();
    SceneGraph( GameObject * root );

    GameObject * getRoot();

    void initializeScene(  );

    void input ( QKeyEvent * key );
    void update( float fixedStep );
    void render( Camera &camera  );
};

#endif // SCENEGRAPH_H
