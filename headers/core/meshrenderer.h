#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QVector3D>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>

#include <vector>

#include "headers/render/mesh.h"

#include "headers/core/camera.h"
#include "headers/core/transform.h"
#include "headers/core/gameComponent.h"

class MeshRenderer : public QOpenGLFunctions_3_1, public GameComponent
{
public:
    MeshRenderer();
    MeshRenderer( Mesh &mesh,  Transform & transform );

    void input() override;
    void update() override;
    void render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) override;

    void renderAABB();

private:
    Mesh mesh;
    Transform &transform;
    QOpenGLBuffer verticesBuffer, indexesBuffer;

    int indexCount;
    bool renderBoundingBox = false;
};

#endif // MESHRENDERER_H
