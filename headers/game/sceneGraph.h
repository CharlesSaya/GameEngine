#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "headers/core/camera.h"
#include "headers/core/gameObject.h"
#include "headers/render/shader.h"

class SceneGraph{
private:
    GameObject * root;

public:

    SceneGraph();
    SceneGraph( GameObject * root );

    GameObject * getRoot();

    void initializeScene(  );

    void input(  GameObject * root,  QKeyEvent * key, float deltaTime );
    void update( GameObject * root );
    void render( GameObject * root,  Camera &camera );
};

#endif // SCENEGRAPH_H
