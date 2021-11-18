#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <QVector2D>
#include <QVector3D>

typedef struct VertexData
{
    QVector3D position = QVector3D();
    QVector3D normal   = QVector3D();
    QVector2D texCoord = QVector2D();
} VertexData;

#endif // VERTEXDATA_H
