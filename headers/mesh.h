#ifndef MESH_H
#define MESH_H

#include <vector>

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QOpenGLTexture>

#include "BasicIO.h"


typedef struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
} VertexData;

class Mesh : protected QOpenGLFunctions_3_1{

 private:
    std::vector<VertexData> vertices;
    std::vector<GLushort> faces ;
    int indexCount;
    QOpenGLTexture * texture;
    QOpenGLBuffer verticesBuffer, indexesBuffer;
    QVector3D meshColor;

 public:
    Mesh();
    Mesh( std::string filepath, QVector3D meshColor );

    void loadGeometry( std::string filepath );
    void drawMesh( QOpenGLShaderProgram *program);
};

#endif // MESH_H
