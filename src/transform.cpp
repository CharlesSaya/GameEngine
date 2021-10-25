#include "headers/transform.h"

Transform::Transform(){
    this->position = QVector3D();
    this->rotation = QQuaternion();
    this->scale = QVector3D(1.,1.,1.);
}

Transform::Transform( QVector3D &translation,  QQuaternion &rotation, QVector3D &scale ){
    this->position = translation;
    this->rotation = rotation;
    this->scale = scale;
}

void Transform::applyTranslation( QVector3D vector ){
    this->position = ( vector );
    this->model.translate( this->position );
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
//    QMatrix4x4 scaleMat, rotateMat, transMat;
//    scaleMat.scale( this->scale );
//    rotateMat.rotate( this->rotation );
//    transMat.translate( this->position );

    return this->model;
}

