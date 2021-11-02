#include "headers/render/mesh.h"

using namespace OBJIO;

Mesh::Mesh(){

}

Mesh::Mesh( std::string filepath, QVector3D meshColor ){

    this->meshColor = meshColor;

    // Generate 2 VBOs
    verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    verticesBuffer.create();
    indexesBuffer.create();

    loadGeometry( filepath );
    initBuffers();
}

void Mesh::loadGeometry( std::string filepath ){

    //Load object
    std::vector<QVector3D> v;
    std::vector< std::vector< unsigned int > > indexes;
    open<QVector3D, unsigned int>( filepath, v, indexes );

    //Create vertex data
    Q_FOREACH( QVector3D vec, v){
        vertices.push_back( { vec, QVector2D(0.0,0.0) } );
    }

    Q_FOREACH( std::vector<unsigned int> vec, indexes ){
        Q_FOREACH( unsigned int i, vec){
            faces.push_back(GLushort( i ));
        }
    }

    this->bBox = AABB( v );
}

void Mesh::initBuffers(){
    initializeOpenGLFunctions();

    indexCount = faces.size() ;

    verticesBuffer.bind();
    verticesBuffer.allocate( vertices.data(), vertices.size() * sizeof(VertexData));

    indexesBuffer.bind();
    indexesBuffer.allocate ( faces.data(), indexCount * sizeof(GLushort));

}


void Mesh::drawMesh( QOpenGLShaderProgram *program ){
    program->setUniformValue("meshColor", this->meshColor );

    verticesBuffer.bind();
    indexesBuffer.bind();

    quintptr offset = 0;

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset , 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);
}



