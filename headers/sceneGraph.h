#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QVector3D>
#include "gameObject.h"

class SceneGraph{
private:
    GameObject * root;

public:

    SceneGraph();
    SceneGraph( GameObject * root );

    GameObject * getRoot();

    void initializeScene(  );
    void draw( GameObject * root,  QMatrix4x4 &view, QMatrix4x4 &projection, QOpenGLShaderProgram * program  );
};

#endif // SCENEGRAPH_H
