#include "headers/transform.h"

Transform::Transform(){
    this->translation = QVector3D();
    this->rotation = QQuaternion();
    this->scale = QVector3D(1.,1.,1.);
}

Transform::Transform( QVector3D &translation,  QQuaternion &rotation, QVector3D &scale ){
    this->translation = translation;
    this->rotation = rotation;
    this->scale = scale;
}

void Transform::applyTranslation( QVector3D vector ){
    this->translation = ( vector );
    this->model.translate( this->translation );
}

void Transform::applyRotation( QQuaternion quaternion ){
    this->rotation  = ( quaternion );
    this->model.rotate( this->rotation );
}

void Transform::applyScale( QVector3D vector ){
    this->scale = ( vector );
    this->model.scale(this->scale);
}

void Transform::resetModel(){
    this->model.setToIdentity();
}

QMatrix4x4 & Transform::getModel()
{
    return this->model;
}

