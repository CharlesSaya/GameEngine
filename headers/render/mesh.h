#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QOpenGLBuffer>
#include <QVector3D>

#include "dirent.h"

#include "headers/core/objloader.h"

#include "headers/render/shader.h"
#include "headers/render/terrain.h"
#include "headers/render/vertexData.h"

#include "headers/render/AABB.h"
#include "headers/render/texture.h"

class Mesh : protected QOpenGLFunctions_3_1{

 private:
    int indexCount;

    std::vector<std::vector<VertexData>> meshesVertexDatas;
    std::vector<std::vector<GLuint>> meshesFaces ;
    std::vector<VertexData> a ;
    std::vector<GLuint> b;

    QOpenGLBuffer verticesBuffer, indexesBuffer;
    std::vector<Texture>  textures;
    QVector3D meshColor;
    Shader * shader;
    AABB bBox;

 public:
    Mesh();
    Mesh( std::string filepath, std::vector<Texture> textures, Shader * shader, QVector3D meshColor );
    Mesh( Terrain terrain, std::vector<Texture> textures, Shader * shader, QVector3D meshColor  );

    void loadGeometry( std::string filepath );
    void initBuffers( uint lod );
    void drawMesh( float distance );
    uint getLod( float distance );

    const QVector3D &getMeshColor() const;
    void setMeshColor(const QVector3D &newMeshColor);

    Shader *getShader() const;
    void setShader(Shader *newShader);

    const std::vector<Texture> &getTextures() const;
    void setTextures(const std::vector<Texture> &newTextures);
    void bindTextures();
    void unbindTextures();

    AABB &getAABB();
};

#endif // MESH_H
