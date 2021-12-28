#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QVector3D>

#include <vector>

#include "headers/render/mesh.h"

#include "headers/core/cameraComponent.h"
#include "headers/core/transform.h"

class MeshRenderer : public QObject
{
Q_OBJECT

public slots:
    void updateAABB( const QMatrix4x4& model  ) ;

public:
    MeshRenderer();
    MeshRenderer( Mesh &mesh, QObject * parent = 0  );

    void renderMesh( Transform & transform, const QMatrix4x4& model, CameraComponent * camera );


    Mesh &getMesh();
    void setMesh(const Mesh &newMesh);

private:
    Mesh mesh;
    QOpenGLBuffer verticesBuffer, indexesBuffer;

    bool renderBoundingBox = false;
};

#endif // MESHRENDERER_H
