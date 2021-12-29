#include "headers/core/cameraOrthoComponent.h"


CameraOrthoComponent::CameraOrthoComponent(){
}

CameraOrthoComponent::CameraOrthoComponent(QVector3D cameraPosition, QVector3D cameraTarget,float right, float left, float top, float bottom ,
                     float zNear, float zFar, QObject * parent){
    this->cameraPosition = cameraPosition;
    this->cameraTarget = cameraTarget;

    this->zNear = zNear;
    this->zFar  = zFar;

    this->right = right;
    this->left = left;
    this->top = top;
    this->bottom = bottom;

    this->cameraForward = (this->cameraTarget - this->cameraPosition).normalized();
//    this->right = QVector3D::crossProduct( this->yAxis ,this->cameraForward).normalized();
//    this->cameraUp = QVector3D::crossProduct( this->cameraForward, this->right );

}

const QMatrix4x4& CameraOrthoComponent::getViewMatrix(){
    view.setToIdentity();
    view.lookAt(this->cameraPosition, this->cameraPosition + this->cameraForward, this->cameraUp);
    return view;
}

void CameraOrthoComponent::setProjection()
{
    this->projection.setToIdentity();
    this->projection.ortho(this->left,this->right,this->bottom,this->top,this->zNear,this->zFar);
}

const QMatrix4x4 &CameraOrthoComponent::getProjection() const
{
    return projection;
}
