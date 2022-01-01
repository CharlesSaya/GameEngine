#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLFunctions_3_3_Core>

#include <headers/core/gameObjectCamera.h>

#include "headers/render/texture.h"
#include "headers/render/vertexData.h"


class CubeMap : protected QOpenGLFunctions_3_3_Core{

public:

    CubeMap();
    CubeMap( int size, Shader * shader, std::vector< Texture> & textures );
    void initGeometry();
    void initTextures();
    void bindTexture( uint textureUnit );
    void unbindTexture();
    void render( GameObjectCamera * camera, QMatrix4x4 model );

private:
    unsigned int textureID = 0;

    int size, indicesCount = 36 ;
    std::vector< Texture> textures;
    QOpenGLBuffer verticesBuffer, indicesBuffer;
    QOpenGLTexture * skyboxTexture;
    std::vector<VertexData> cubeVertices;
    Shader * shader;

};

#endif // CUBEMAP_H
