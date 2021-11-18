#include "headers/core/transform.h"

Transform::Transform(){
    this->translation = QVector3D();
    this->rotation = QQuaternion();
    this->scale = QVector3D(1.,1.,1.);
    this->position = QVector3D();
}

Transform::Transform( QVector3D &position ){
    this->position = position;
}

void Transform::applyTranslation( QVector3D vector ){
    this->position += vector;
}

void Transform::applyRotation( QQuaternion quaternion ){
    this->rotation  = quaternion;
}

void Transform::applyScale( QVector3D vector ){
    this->scale = ( vector );
}

void Transform::resetModel(){
    this->model.setToIdentity();
}

QMatrix4x4 & Transform::getModel()
{
    QMatrix4x4 tr, rt, sc;
    tr.translate( this->position );
    rt.rotate( this->rotation );
    sc.scale( this->scale );
    this->model = tr * rt * sc;
    return this->model;
}

QVector3D &Transform::getPosition()
{
    return position;
}

void Transform::setPosition(const QVector3D &newPosition)
{
    position = newPosition;
}

QVector3D Transform::getWorldPosition(){
    return this->getModel() * this->position;
}
