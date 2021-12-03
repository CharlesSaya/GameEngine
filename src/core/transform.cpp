#include "headers/core/transform.h"

Transform::Transform( QObject * parent ) : QObject(parent) {
    this->translation = QVector3D();
    this->rotation = QQuaternion();
    this->scale = QVector3D(1.,1.,1.);
    this->position = QVector3D();
}

Transform::Transform( QVector3D &position ){
    this->position = position;
}

Transform::Transform( Transform & transform ){
    this->translation = transform.translation;
    this->rotation = transform.rotation;
    this->scale = transform.scale;
    this->position = transform.position;
}


void Transform::applyTranslation( QVector3D vector ){
    this->position += vector;
    emit transformed();
}

void Transform::applyRotation( QQuaternion quaternion ){
    this->rotation  = quaternion;
    emit transformed();
}

void Transform::applyScale( QVector3D vector ){
    this->scale = ( vector );
    emit transformed();
}

void Transform::resetModel(){
    this->model.setToIdentity();
    emit transformed();
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
