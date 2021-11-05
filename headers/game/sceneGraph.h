#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>

#include "../core/gameObject.h"

class SceneGraph{
private:
    GameObject * root;

public:

    SceneGraph();
    SceneGraph( GameObject * root );

    GameObject * getRoot();

    void initializeScene(  );

    void input();
    void update();
    void render( GameObject * root,  QMatrix4x4 &view, QMatrix4x4 &projection, QOpenGLShaderProgram * program  );
};

#endif // SCENEGRAPH_H
