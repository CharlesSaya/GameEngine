#include "headers/core/camera.h"


Camera::Camera(){
}

Camera::Camera(QVector3D cameraPosition, QVector3D cameraTarget, float fov, float zNear, float zFar ){
    this->cameraPosition = cameraPosition;
    this->cameraTarget = cameraTarget;

    this->fov   = fov;
    this->zNear = zNear;
    this->zFar  = zFar;

    this->cameraForward = (this->cameraTarget - this->cameraPosition).normalized();
    this->right = QVector3D::crossProduct( this->yAxis ,this->cameraForward).normalized();
    this->cameraUp = QVector3D::crossProduct( this->cameraForward, this->right );

}

bool Camera::eventFilter(QObject *obj, QEvent *event){
    return false;
}

const QMatrix4x4 &Camera::getProjection() const
{
    return projection;
}

void Camera::setProjection( float aspect )
{
    this->projection.setToIdentity();
    this->projection.perspective( fov, aspect, zNear, zFar );
}

const QMatrix4x4& Camera::getViewMatrix(){
    view.setToIdentity();
    view.lookAt(this->cameraPosition, this->cameraPosition + this->cameraForward, this->cameraUp);
    return view;

}

const QVector3D& Camera::getRight(){
    right =  QVector3D::crossProduct( this->yAxis ,this->cameraForward ).normalized();
    return right;
}

const QVector3D& Camera::getLeft(){
    left = QVector3D::crossProduct( this->cameraForward,  this->yAxis ).normalized();
    return left;

}

void Camera::move( QVector3D axis ){
    this->cameraPosition += axis;
    this->cameraTarget += axis;
}

void Camera::rotate( float pitch, float yaw ){

    this->cameraForward.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    this->cameraForward.setY( sin(qDegreesToRadians(pitch)) );
    this->cameraForward.setZ( sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    this->cameraForward.normalize();
    this->cameraUp =  QVector3D::crossProduct( this->cameraForward, getRight() ).normalized();

}

const QVector3D& Camera::getCameraForward()
{
    return cameraForward;
}

const QVector3D& Camera::getCameraPosition()
{
    return cameraPosition;
}

