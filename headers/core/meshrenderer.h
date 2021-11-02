#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QVector3D>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>

#include <vector>

#include "../render/mesh.h"
#include "gameComponent.h"

class MeshRenderer : public QOpenGLFunctions_3_1, public GameComponent
{
public:
    MeshRenderer();
    MeshRenderer( QOpenGLShaderProgram * program, Mesh &mesh );
    void render();

private:
    Mesh mesh;
    QOpenGLShaderProgram * program;

    bool renderAABB = false;
};

#endif // MESHRENDERER_H
