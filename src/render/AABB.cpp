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
    minDefault = min;
    maxDefault = max;

}

void AABB::resizeAABB( AABB& bBox ){

    if (min.x() > bBox.getMin().x())
        min.setX( bBox.getMin().x() );

    if (min.y() > bBox.getMin().y())
        min.setY( bBox.getMin().y() );

    if (min.z() > bBox.getMin().z())
        min.setZ( bBox.getMin().z() );

    if (max.x() < bBox.getMin().x())
        max.setX( bBox.getMin().x() );

    if (max.y() < bBox.getMin().y())
        max.setY( bBox.getMin().y() );

    if (max.z() < bBox.getMin().z())
        max.setZ( bBox.getMin().z() );

    if (min.x() > bBox.getMax().x())
        min.setX( bBox.getMax().x() );

    if (min.y() > bBox.getMax().y())
        min.setY( bBox.getMax().y() );

    if (min.z() > bBox.getMax().z())
        min.setZ( bBox.getMax().z() );

    if (max.x() < bBox.getMax().x())
        max.setX( bBox.getMax().x() );

    if (max.y() < bBox.getMax().y())
        max.setY( bBox.getMax().y() );

    if (max.z() < bBox.getMax().z())
        max.setZ( bBox.getMax().z());

    minDefault = min;
    maxDefault = max;
}

void AABB::resetAABB(){
    min = QVector3D();
    max = QVector3D();
}

void AABB::updateAABB( const  QMatrix4x4 &model ) {
    min = model * minDefault;
    max = model * maxDefault;
}

void AABB::initBuffers(){

    QVector3D minD = minDefault;
    QVector3D maxD = maxDefault;

    float diffX = maxD.x() - minD.x();
    float diffY = maxD.y() - minD.y();
    float diffZ = maxD.z() - minD.z();

    vertices = {
                minD,
                minD + QVector3D( diffX, 0., 0.),
                minD + QVector3D( diffX, 0., diffZ),
                minD + QVector3D( 0., 0., diffZ),

                minD + QVector3D( 0., diffY, 0.),
                minD + QVector3D( diffX,diffY, 0.),
                minD + QVector3D( diffX, diffY, diffZ),
                minD + QVector3D( 0., diffY, diffZ)

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
              7, 4
            };

    indexCount = lines.size() ;


}

//void AABB::drawAABB( Shader * shader ){
//    this->initBuffers();

//    indexCount = this->getIndexCount();

//    AABBverticesBuffer.bind();
//    AABBverticesBuffer.allocate( this->getVertices().data(), this->getVertices().size() * sizeof( QVector3D ) );

//    AABBindexesBuffer.bind();
//    AABBindexesBuffer.allocate ( this->getLines().data(), indexCount * sizeof( GLuint ) );

//    quintptr offset = 0;

//    int vertexLocation = shader->getProgram().attributeLocation("a_position");
//    shader->getProgram().enableAttributeArray(vertexLocation);
//    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

//       glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
//}

float AABB::getHeight(){
    return this->max.y() - this->min.y();
}

float AABB::getWidth(){
    return this->max.x() - this->min.x();
}

QVector3D &AABB::getMax()
{
   return max;
}

void AABB::setMax(const QVector3D &newMax)
{
   max = newMax;
}

QVector3D &AABB::getMin()
{
   return min;
}

void AABB::setMin(const QVector3D &newMin)
{
    min = newMin;
}

int AABB::getIndexCount() const
{
    return indexCount;
}

void AABB::setIndexCount(int newIndexCount)
{
    indexCount = newIndexCount;
}

const std::vector<QVector3D> &AABB::getVertices() const
{
    return vertices;
}

void AABB::setVertices(const std::vector<QVector3D> &newVertices)
{
    vertices = newVertices;
}

const std::vector<GLuint> &AABB::getLines() const
{
    return lines;
}

void AABB::setLines(const std::vector<GLuint> &newLines)
{
    lines = newLines;
}
