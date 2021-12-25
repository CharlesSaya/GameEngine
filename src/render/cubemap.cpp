#include "headers/render/cubemap.h"

CubeMap::CubeMap(){

}

CubeMap::CubeMap( int size, Shader * shader, std::vector< Texture > & textures ){
    this->size = size;
    this->shader = shader;
    this->textures = textures;

    this->verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->indicesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    this->verticesBuffer.create();
    this->indicesBuffer.create();
    initializeOpenGLFunctions();

    initGeometry();
    initTextures();
}

void CubeMap::initGeometry(){

    // TOP

    cubeVertices.push_back( { QVector3D( -size, size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D(  1.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 1.0 )
                            } );

    // LEFT

    cubeVertices.push_back( { QVector3D( -size, -size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, - size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 1.0 )
                            } );

    // BOTTOM

    cubeVertices.push_back( { QVector3D( -size, - size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, - size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, - size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, - size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 0.0 )
                            } );

    // RIGHT

    cubeVertices.push_back( { QVector3D( size, -size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, - size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 1.0 )
                            } );

    // FRONT

    cubeVertices.push_back( { QVector3D( -size, -size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, - size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, size, size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 1.0 )
                            } );

    // BACK

    cubeVertices.push_back( { QVector3D( size, -size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, -size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 0.0 )
                            } );

    cubeVertices.push_back( { QVector3D( -size, size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 0.0, 1.0 )
                            } );

    cubeVertices.push_back( { QVector3D( size, size, -size ),
                              QVector3D( -1.0, 0.0, 0.0),
                              QVector2D( 1.0, 1.0 )
                            } );

    verticesBuffer.create();
    indicesBuffer.create();

    int indices[] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
    this->verticesBuffer.bind();
    this->verticesBuffer.allocate( cubeVertices.data(), cubeVertices.size() * sizeof(VertexData));

    this->indicesBuffer.bind();
    this->indicesBuffer.allocate ( indices, this->indicesCount  * sizeof(GLuint));


}

void CubeMap::initTextures(){
    this->skyboxTexture = new QOpenGLTexture( QOpenGLTexture::TargetCubeMap );
    this->skyboxTexture->create();
    this->skyboxTexture->setSize(2048, 2048);
    this->skyboxTexture->setFormat(QOpenGLTexture::RGBAFormat);

    this->skyboxTexture->allocateStorage();


    this->skyboxTexture->setData( 0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                                  textures[0].getImage().mirrored().constBits());

    this->skyboxTexture->setData( 0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                                  textures[4].getImage().mirrored( true, false ).constBits());

    this->skyboxTexture->setData( 0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                                  textures[5].getImage().mirrored().constBits());

    this->skyboxTexture->setData( 0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                                  textures[3].getImage().mirrored().constBits());

    this->skyboxTexture->setData( 0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                                  textures[1].getImage().mirrored(true, false).constBits());

    this->skyboxTexture->setData( 0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                                  textures[2].getImage().mirrored().constBits());

    this->skyboxTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    this->skyboxTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    this->skyboxTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    this->skyboxTexture->generateMipMaps();
}

void CubeMap::bindTexture( uint textureUnit ){

    this->skyboxTexture->bind( textureUnit );
    shader->setUniformValue( "skybox", textureUnit );
}

void CubeMap::unbindTexture(){
    this->skyboxTexture->release();
}

void CubeMap::render( GameObjectCamera * camera, QMatrix4x4 model ){
    shader->useShaderProgram();

    shader->setUniformValue( "model", model );
    shader->setUniformValue( "view", camera->getCameraComponent()->getViewMatrix() );
    shader->setUniformValue( "projection", camera->getCameraComponent()->getProjection() );

    this->verticesBuffer.bind();
    this->indicesBuffer.bind();

    this->bindTexture( 0 );

    quintptr offset = 0;

    int vertexLocation = this->shader->getProgram().attributeLocation("a_position");
    this->shader->getProgram().enableAttributeArray(vertexLocation);
    this->shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int normalLocation = this->shader->getProgram().attributeLocation("a_normal");
    this->shader->getProgram().enableAttributeArray(normalLocation);
    this->shader->getProgram().setAttributeBuffer(normalLocation, GL_FLOAT, offset , 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLocation = this->shader->getProgram().attributeLocation("a_texcoord");
    this->shader->getProgram().enableAttributeArray(texcoordLocation);
    this->shader->getProgram().setAttributeBuffer(texcoordLocation, GL_FLOAT, offset , 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, 36 , GL_UNSIGNED_INT, 0);

    this->unbindTexture();

}
