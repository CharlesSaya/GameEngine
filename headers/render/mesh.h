#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>

#include "dirent.h"

#include "headers/core/objloader.h"
#include "headers/core/transform.h"

#include "headers/render/shader.h"
#include "headers/render/terrain.h"
#include "headers/render/vertexData.h"

#include "headers/render/AABB.h"
#include "headers/render/texture.h"

class Mesh : protected QOpenGLFunctions_3_3_Core{

 private:
    int indexCount;

    std::vector<std::vector<VertexData>> meshesVertexDatas;
    std::vector<std::vector<GLuint>> meshesFaces ;
    std::vector<VertexData> a ;
    std::vector<GLuint> b;

    QOpenGLBuffer verticesBuffer, indexesBuffer;
    QOpenGLBuffer AABBverticesBuffer, AABBindexesBuffer;
    std::vector<Texture>  textures;
    uint m_shadowMapTex;
    QVector3D meshColor;
    Shader * shader;
    AABB bBox;
    bool renderAABB;
    bool terrain;
 public:
    Mesh();
    Mesh( std::string filepath, std::vector<Texture> textures, Shader * shader, QVector3D meshColor, bool renderAABB = false );
    Mesh( Terrain& terrain, std::vector<Texture> textures, Shader * shader, QVector3D meshColor, bool renderAABB = false );

    void loadGeometry( std::string filepath );
    void initBuffers( uint lod );
    void drawMesh( float distance, Shader * gShader );
    void drawAABB( Shader * shader );
    uint getLod( float distance );

    bool isTerrain();

    void updateAABB( const QMatrix4x4& model  ) ;

    const QVector3D &getMeshColor() const;
    void setMeshColor(const QVector3D &newMeshColor);

    Shader *getShader() const;
    void setShader(Shader *newShader);

    const std::vector<Texture> &getTextures() const;
    void setTextures(const std::vector<Texture> &newTextures);

    void bindTextures( Shader * shader );
    void bindShadowTexture();
    void unbindTextures();
    void unbindShadowTexture();

    AABB &getAABB();
    bool getRenderAABB() const;

    void setShadowTexture(uint m_shadowMapTex);

    void setTerrain(bool newTerrain);
    const std::vector<std::vector<VertexData> > &getMeshesVertexDatas() const;
};

#endif // MESH_H
