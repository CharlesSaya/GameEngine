#include "headers/render/ray.h"

void Ray::initBuffers(){
    std::vector<QVector3D> vertices = { _origin, _origin + 10. * _direction };
    this->verticesBuffer.bind();
    this->verticesBuffer.allocate( vertices.data(), vertices.size() * sizeof(QVector3D));

}

/**
 * Draw the ray  (telekinesis)
 */
void Ray::drawRay( Shader * shader, const QMatrix4x4& view, const QMatrix4x4& projection  ){
    initBuffers();

    shader->useShaderProgram();
    shader->setUniformValue( "model", QMatrix4x4() );
    shader->setUniformValue( "view", view );
    shader->setUniformValue( "projection", projection );

    this->verticesBuffer.bind();

    int vertexLocation = shader->getProgram().attributeLocation("a_position");
    shader->getProgram().enableAttributeArray(vertexLocation);
    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawArrays(GL_LINES, 0, 2 );
}

void Ray::setDirection(const QVector3D &newDirection)
{
    _direction = newDirection;
}

const QVector3D& Ray::getOrigin() const
{
    return _origin;
}

const QVector3D& Ray::getDirection() const
{
    return _direction;
}
