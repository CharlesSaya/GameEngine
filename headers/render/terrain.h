/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TERRAIN_H
#define TERRAIN_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QtMath>

#include "texture.h"
#include "vertexData.h"

class Terrain
{
public:
    Terrain();
    Terrain( float gridSize, float scale, std::string file, Texture & heightmap );

    float getHeight( QVector2D coords );
    float getHeightOfTerrain( QVector3D &position );
    float baricentricHeight( QVector3D v0, QVector3D v1, QVector3D v2, QVector2D pos );

    float getHeightTerrain(  QVector3D &position );

    std::vector<VertexData> &getPlaneVertices();
    void setPlaneVertices(const std::vector<VertexData> &newPlaneVertices);

    std::vector<GLuint> &getPlaneIndices();
    void setPlaneIndices(const std::vector<GLuint> &newPlaneIndices);

    float getMinimumHeight();
    float getMaximumHeight();

    QVector3D computeNormal( QVector2D texcoords );
    QVector3D computeFaceNormal( QVector3D& v0, QVector3D&  v1, QVector3D& v2  );

    float getScale() const;

    void setMaximumHeight(float newHeight);

    QVector3D getFaceNormalAtPosition( QVector3D & worldPosition );

    const std::string &getOBJFilename() const;

    float getMaxClimbableHeight() const;

private:
    void initGeometry();

    int width, height;
    float maximumHeight;
    float gridSize = 128.f;
    float gridSquareSize , gridNumber;
    float scale;

    float maxClimbableHeight = 15.f;

    std::vector< VertexData > planeVertices;
    std::vector< GLuint > planeIndices;
    std::vector< float > heights;
    std::map< int, std::vector<int>> map;
    Texture heightmap;
    std::string terrainOBJ;
};

#endif // TERRAIN_H
