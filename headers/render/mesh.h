#ifndef MESH_H
#define MESH_H

#include <vector>

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QOpenGLBuffer>
#include <QVector3D>

#include "AABB.h"
#include "../core/BasicIO.h"


typedef struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
} VertexData;

class Mesh : protected QOpenGLFunctions_3_1{

 private:
    int indexCount;

    std::vector<VertexData> vertices;
    std::vector<GLushort> faces ;
    QOpenGLTexture * texture;
    QOpenGLBuffer verticesBuffer, indexesBuffer;
    QVector3D meshColor;
    AABB bBox;

 public:
    Mesh();
    Mesh( std::string filepath, QVector3D meshColor );

    void loadGeometry( std::string filepath );
    void initBuffers();
    void drawMesh( QOpenGLShaderProgram *program);
    void drawAABB( QOpenGLShaderProgram *program);
};

#endif // MESH_H
