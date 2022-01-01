#include "headers/core/transform.h"

Transform::Transform( QObject * parent ) : QObject(parent) {
    this->rotation = QQuaternion();
    this->scale = QVector3D(1.,1.,1.);
    this->position = QVector3D();
    this->lastWorldPosition = QVector3D();
}

Transform::Transform( QVector3D &position ){
    this->position = position;
}

Transform::Transform( Transform & transform ){
    this->rotation = transform.rotation;
    this->scale = transform.scale;
    this->position = transform.position;
    this->lastWorldPosition = transform.position;
}


void Transform::applyTranslation( QVector3D vector ){
    this->lastWorldPosition = this->getWorldPosition();
    this->position += vector;
    emit transformed();
}


void Transform::applyRotation( QQuaternion quaternion ){
    this->lastWorldPosition = this->getWorldPosition();
    this->rotation = quaternion;
}

void Transform::applyScale( QVector3D vector ){
    this->lastWorldPosition = this->getWorldPosition();
    this->scale = ( vector );
    emit transformed();
}

void Transform::applyScale( float scale ){
    this->lastWorldPosition = this->getWorldPosition();
    this->scale = QVector3D(scale,scale,scale);
    emit transformed();
}


void Transform::resetModel(){
    this->model.setToIdentity();
    this->rotation = QQuaternion();
    this->scale = QVector3D(1.,1.,1.);
    this->position = QVector3D();
    this->lastWorldPosition = this->getWorldPosition();
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

QQuaternion &Transform::getRotation()
{
    return rotation;
}

QVector3D &Transform::getScale()
{
    return scale;
}

void Transform::setPosition(const QVector3D &newPosition)
{
    this->position = newPosition;
    emit transformed();

}

QVector3D& Transform::getWorldPosition(){

    worldPosition =  this->getModel() * QVector3D(0.0, 0.0, 0.0);
    return worldPosition;
}
