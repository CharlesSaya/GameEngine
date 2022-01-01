#ifndef RAY_H
#define RAY_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>

#include "headers/render/shader.h"

class Ray :  protected QOpenGLFunctions_3_3_Core{

public:

    Ray() {

        initializeOpenGLFunctions();
        verticesBuffer = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
        verticesBuffer.create();

        verticesBuffer = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
        verticesBuffer.create();

    }

    Ray(const QVector3D &origin, const QVector3D &direction) : _origin(origin),_direction(direction)
    {

        initializeOpenGLFunctions();
        verticesBuffer = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
        verticesBuffer.create();

        verticesBuffer = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
        verticesBuffer.create();

    }

    const QVector3D& getOrigin() const ;
    const QVector3D& getDirection() const;

    void initBuffers();
    void drawRay( Shader * shader, const QMatrix4x4& view, const QMatrix4x4& projection  );
private:

    QVector3D _origin;
    QVector3D _direction;

    QOpenGLBuffer verticesBuffer, indicesBuffer;

};

#endif // RAY_H
