#include "headers/render/AABB.h"

#include <vector>

    AABB::AABB(){

    }

    AABB::AABB( std::vector<VertexData> &points ){
//        initializeOpenGLFunctions();

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


//        verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//        indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

//        verticesBuffer.create();
//        indexesBuffer.create();
//        initBuffers();

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

//    void AABB::initBuffers(){

//        QVector3D min = this->getMin();
//        QVector3D max = this->getMax();

//        float diffX = max.x() - min.x();
//        float diffY = max.y() - min.y();
//        float diffZ = max.z() - min.z();


//        std::vector<QVector3D> vertices = {
//                                            min, min + QVector3D( diffX, 0., 0.),
//                                            min + QVector3D( diffX, 0., diffZ), min + QVector3D( 0., 0., diffZ),

//                                            min + QVector3D( 0., diffY, 0.), min + QVector3D( diffX,diffY, 0.),
//                                            min + QVector3D( diffX, diffY, diffZ), min + QVector3D( 0., diffY, diffZ)

//                                         };

//        lines = { 0, 1,
//                  1, 2,
//                  2, 3,
//                  3, 0,

//                  0, 4,
//                  1, 5,
//                  2, 6,
//                  3, 7,

//                  4, 5,
//                  5, 6,
//                  6, 7,
//                  7, 4 };

//        indexCount = lines.size() ;

//        verticesBuffer.bind();
//        verticesBuffer.allocate( vertices.data(), vertices.size() * sizeof( QVector3D ) );

//        indexesBuffer.bind();
//        indexesBuffer.allocate ( lines.data(), indexCount * sizeof( GLuint ) );
//    }

//    void AABB::drawAABB( QOpenGLShaderProgram * program ){

//        verticesBuffer.bind();
//        indexesBuffer.bind();

//        quintptr offset = 0;

//        int vertexLocation = program->attributeLocation("a_position");
//        program->enableAttributeArray(vertexLocation);
//        program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

//        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);
//    }

