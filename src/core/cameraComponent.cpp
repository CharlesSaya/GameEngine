#include "headers/core/cameraComponent.h"


CameraComponent::CameraComponent(){
}

CameraComponent::CameraComponent(QVector3D cameraPosition, QVector3D cameraTarget, float fov, float zNear, float zFar, QObject * parent ){
    this->cameraPosition = cameraPosition;
    this->cameraTarget = cameraTarget;

    this->fov   = fov;
    this->zNear = zNear;
    this->zFar  = zFar;

    this->cameraForward = (this->cameraTarget - this->cameraPosition).normalized();
    this->right = QVector3D::crossProduct( this->yAxis ,this->cameraForward).normalized();
    this->cameraUp = QVector3D::crossProduct( this->cameraForward, this->right );
}

CameraComponent::CameraComponent(QVector3D cameraPosition, QVector3D cameraTarget,float left, float right, float bottom, float top ,
                     float zNear, float zFar, QObject * parent){

    this->cameraPosition = cameraPosition;
    this->cameraTarget = cameraTarget;

    this->zNear = zNear;
    this->zFar  = zFar;

    this->right_O = right;
    this->left_O = left;
    this->top_O = top;
    this->bottom_O = bottom;

    this->cameraForward = (this->cameraTarget - this->cameraPosition).normalized();
    this->right = QVector3D::crossProduct( this->yAxis ,this->cameraForward).normalized();
    this->cameraUp = QVector3D::crossProduct( this->cameraForward, this->right );

}


void CameraComponent::updatePosition(const QMatrix4x4& model)
{
    this->cameraPosition = model * QVector3D(0.0f,5.0f,8.0f);
}
bool CameraComponent::eventFilter(QObject *obj, QEvent *event){
    return false;
}

const QVector3D &CameraComponent::getCameraUp() const
{
    return cameraUp;
}

const QMatrix4x4 &CameraComponent::getProjection() const
{
    return projection;
}

void CameraComponent::setProjectionPersp( float aspect )
{
    this->projection.setToIdentity();
    this->projection.perspective( fov, aspect, zNear, zFar );
}

void CameraComponent::setProjectionOrtho()
{
    this->projection.setToIdentity();
    this->projection.ortho(this->left_O,this->right_O,this->bottom_O,this->top_O,this->zNear,this->zFar);
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

void CameraComponent::setCameraTarget(QVector3D cameraTarget)
{
    this->cameraForward = (cameraTarget - this->cameraPosition).normalized();

}

const QVector3D& CameraComponent::getCameraForward()
{
    return cameraForward;
}

const QVector3D& CameraComponent::getCameraPosition()
{
    return cameraPosition;
}

const QVector3D& CameraComponent::getCameraTarget()
{
    return cameraTarget;
}

