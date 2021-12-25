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

        if (min.z() < points[i].position.z())
            min.setZ( points[i].position.z() );

        if (max.x() < points[i].position.x())
            max.setX( points[i].position.x() );

        if (max.y() < points[i].position.y())
            max.setY( points[i].position.y() );

        if (max.z() > points[i].position.z())
            max.setZ( points[i].position.z() );
    }
    minDefault = min;
    maxDefault = max;
    lastMin = min;
    lastMax = max;
}

void AABB::resizeAABB( AABB& bBox ){
    if (min.x() > bBox.getMin().x())
        min.setX( bBox.getMin().x() );

    if (min.y() > bBox.getMin().y())
        min.setY( bBox.getMin().y() );

    if (min.z() < bBox.getMin().z())
        min.setZ( bBox.getMin().z() );

    if (max.x() < bBox.getMax().x())
        max.setX( bBox.getMax().x() );

    if (max.y() < bBox.getMax().y())
        max.setY( bBox.getMax().y() );

    if (max.z() > bBox.getMax().z())
        max.setZ( bBox.getMax().z());


    minDefault = min;
    maxDefault = max;
    lastMin = min;
    lastMax = max;
}

void AABB::resetAABB(){
    min  = QVector3D(__FLT_MAX__ ,__FLT_MAX__, -__FLT_MAX__) ;
    max = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,__FLT_MAX__) ;
    lastMin = QVector3D(__FLT_MAX__ ,__FLT_MAX__, -__FLT_MAX__) ;
    lastMax = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,__FLT_MAX__) ;
}

void AABB::updateAABB( const  QMatrix4x4 &model ) {
    lastMin = min;
    lastMax = max;
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

bool AABB::intersect(const Ray & ray) {

    QVector3D rayDirection = ray.getDirection();
    QVector3D rayOrigin = ray.getOrigin();

    float t0x = (min.x() - rayOrigin.x()) / rayDirection.x();
    float t1x = (max.x() - rayOrigin.x()) / rayDirection.x();

    if (t0x > t1x)
        std::swap(t0x, t1x);

    float tMin = t0x;
    float tMax = t1x;

    float t0y = (min.y() - rayOrigin.y()) / rayDirection.y();
    float t1y = (max.y() - rayOrigin.y()) / rayDirection.y();

    if (t0y > t1y)
        std::swap(t0y, t1y);

    if (tMin > t1y || tMax > t1x)
        return false;

    tMin = (t0x > t0y) ? t0x : t0y;
    tMax = (t1x > t1y) ? t1y : t1x;

    float t0z = (min.z() - rayOrigin.z()) / rayDirection.z();
    float t1z = (max.z() - rayOrigin.z()) / rayDirection.z();

    if (t0z > t1z)
        std::swap(t0z, t1z);

    if (tMin > t1z || t0z > tMax)
        return false;

    if (t0z > tMin)
        tMin = t0z;

    if (t1z < tMax)
        tMax = t1z;

    return true;

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

float AABB::getDepth(){
    return this->max.z() - this->min.z();
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

 QVector3D &AABB::getMinDefault()
{
    return minDefault;
}

void AABB::setMinDefault(const QVector3D &newMinDefault)
{
    minDefault = newMinDefault;
}

 QVector3D &AABB::getMaxDefault()
{
    return maxDefault;
}

void AABB::setMaxDefault(const QVector3D &newMaxDefault)
{
    maxDefault = newMaxDefault;
}
const QVector3D &AABB::getLastMin() const
{
    return lastMin;
}

void AABB::setLastMin(const QVector3D &newLastMin)
{
    lastMin = newLastMin;
}

const QVector3D &AABB::getLastMax() const
{
    return lastMax;
}

void AABB::setLastMax(const QVector3D &newLastMax)
{
    lastMax = newLastMax;
}
