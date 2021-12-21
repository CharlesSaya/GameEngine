#include "headers/core/cameraComponent.h"


CameraComponent::CameraComponent(){
}

CameraComponent::CameraComponent(QVector3D cameraPosition, QVector3D cameraTarget, float fov, float zNear, float zFar ){
    this->cameraPosition = cameraPosition;
    this->cameraTarget = cameraTarget;

    this->fov   = fov;
    this->zNear = zNear;
    this->zFar  = zFar;

    this->cameraForward = (this->cameraTarget - this->cameraPosition).normalized();
    this->right = QVector3D::crossProduct( this->yAxis ,this->cameraForward).normalized();
    this->cameraUp = QVector3D::crossProduct( this->cameraForward, this->right );

}

void CameraComponent::updatePosition(Transform & transform )
{
    this->cameraPosition =this->cameraPosition * transform.getModel();
}

bool CameraComponent::eventFilter(QObject *obj, QEvent *event){
    return false;
}

const QMatrix4x4 &CameraComponent::getProjection() const
{
    return projection;
}

void CameraComponent::setProjection( float aspect )
{
    this->projection.setToIdentity();
    this->projection.perspective( fov, aspect, zNear, zFar );
}

const QMatrix4x4& CameraComponent::getViewMatrix(){
    view.setToIdentity();
    view.lookAt(this->cameraPosition, this->cameraPosition + this->cameraForward, this->cameraUp);
    return view;

}

const QVector3D& CameraComponent::getRight(){
    right =  QVector3D::crossProduct( this->yAxis ,this->cameraForward ).normalized();
    return right;
}

const QVector3D& CameraComponent::getLeft(){
    left = QVector3D::crossProduct( this->cameraForward,  this->yAxis ).normalized();
    return left;

}

void CameraComponent::move( QVector3D axis ){
    this->cameraPosition += axis;
    this->cameraTarget += axis;
}

void CameraComponent::rotate( float pitch, float yaw ){

    this->cameraForward.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    this->cameraForward.setY( sin(qDegreesToRadians(pitch)) );
    this->cameraForward.setZ( sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    this->cameraForward.normalize();
    this->cameraUp =  QVector3D::crossProduct( this->cameraForward, getRight() ).normalized();

}

const QVector3D& CameraComponent::getCameraForward()
{
    return cameraForward;
}

const QVector3D& CameraComponent::getCameraPosition()
{
    return cameraPosition;
}
