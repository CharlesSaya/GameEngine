#include "headers/render/AABB.h"

#include <vector>

AABB::AABB(){

}

AABB::AABB( std::vector<VertexData> &points ){

    for( unsigned int i = 0; i < points.size(); i++){
        if (min.x() > points[i].position.x())
            min.setX( points[i].position.x() );

        if (min.y() > points[i].position.y())
            min.setY( points[i].position.y() );

        if (min.z() > points[i].position.z())
            min.setZ( points[i].position.z() );

        if (max.x() < points[i].position.x())
            max.setX( points[i].position.x() );

        if (max.y() < points[i].position.y())
            max.setY( points[i].position.y() );

        if (max.z() < points[i].position.z())
            max.setZ( points[i].position.z() );
    }


    verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    verticesBuffer.create();
    indexesBuffer.create();

//    initializeOpenGLFunctions();
//    initBuffers();

}

void AABB::resizeAABB( AABB &box ){


    if (min.x() > box.getMin().x())
        min.setX( box.getMin().x() );

    if (min.y() > box.getMin().y())
        min.setY( box.getMin().y() );

    if (min.z() > box.getMin().z())
        min.setZ( box.getMin().z() );

    if (max.x() < box.getMin().x())
        max.setX( box.getMin().x() );

    if (max.y() < box.getMin().y())
        max.setY( box.getMin().y() );

    if (max.z() < box.getMin().z())
        max.setZ( box.getMin().z() );

    if (min.x() > box.getMax().x())
        min.setX( box.getMax().x() );

    if (min.y() > box.getMax().y())
        min.setY( box.getMax().y() );

    if (min.z() > box.getMax().z())
        min.setZ( box.getMax().z() );

    if (max.x() < box.getMax().x())
        max.setX( box.getMax().x() );

    if (max.y() < box.getMax().y())
        max.setY( box.getMax().y() );

    if (max.z() < box.getMax().z())
        max.setZ( box.getMax().z() );


}


void AABB::transformAABB( QMatrix4x4 model ){
    this->max = model * this->max;
    this->min = model * this->min;
}

float AABB::getHeight(){
    return this->max.y() - this->min.y();
}

float AABB::getWidth(){
    return this->max.x() - this->min.x();
}

const QVector3D &AABB::getMax() const
{
   return max;
}

void AABB::setMax(const QVector3D &newMax)
{
   max = newMax;
}

const QVector3D &AABB::getMin() const
{
   return min;
}

void AABB::setMin(const QVector3D &newMin)
{
    min = newMin;
}

void AABB::initBuffers(){

    QVector3D min = this->getMin();
    QVector3D max = this->getMax();

    float diffX = max.x() - min.x();
    float diffY = max.y() - min.y();
    float diffZ = max.z() - min.z();


    std::vector<QVector3D> vertices = {
                                        min,
                                        min + QVector3D( diffX, 0., 0.),
                                        min + QVector3D( diffX, 0., diffZ),
                                        min + QVector3D( 0., 0., diffZ),

                                        min + QVector3D( 0., diffY, 0.),
                                        min + QVector3D( diffX,diffY, 0.),
                                        min + QVector3D( diffX, diffY, diffZ),
                                        min + QVector3D( 0., diffY, diffZ)

                                     };

    lines = { 0, 1,
              1, 2,
              2, 3,
              3, 0,

              0, 4,
              1, 5,
              2, 6,
              3, 7,

              4, 5,
              5, 6,
              6, 7,
              7, 4 };

    indexCount = lines.size() ;

    verticesBuffer.bind();
    verticesBuffer.allocate( vertices.data(), vertices.size() * sizeof( QVector3D ) );

    indexesBuffer.bind();
    indexesBuffer.allocate ( lines.data(), indexCount * sizeof( GLuint ) );
}

void AABB::drawAABB( Shader * shader ){

    this->initBuffers();
    verticesBuffer.bind();
    indexesBuffer.bind();

    quintptr offset = 0;

    int vertexLocation = shader->getProgram().attributeLocation("a_position");
    shader->getProgram().enableAttributeArray(vertexLocation);
    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

//        glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
}

