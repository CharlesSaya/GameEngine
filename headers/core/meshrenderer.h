#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QVector3D>

#include <vector>

#include "headers/render/mesh.h"

#include "headers/core/camera.h"
#include "headers/core/transform.h"

class MeshRenderer : public QObject
{
Q_OBJECT

public slots:
    void updateBBox( Transform * transform );

public:
    MeshRenderer();
    MeshRenderer( Mesh &mesh );

    void renderMesh( Transform & transform, const QMatrix4x4& model, Camera & camera );

    Mesh &getMesh();
    void setMesh(const Mesh &newMesh);

private:
    Mesh mesh;
    QOpenGLBuffer verticesBuffer, indexesBuffer;

    bool renderBoundingBox = false;
};

#endif // MESHRENDERER_H
