#include "headers/core/cameraComponent.h"


CameraComponent::CameraComponent(){
}

/**
 * @brief Constructeur d'une caméra perspective
 * @param cameraPosition
 * @param cameraTarget
 * @param fov
 * @param zNear
 * @param zFar
 * @param parent
 */

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

/**
 * @brief Constructeur d'une caméra orthogonale
 * @param cameraPosition
 * @param cameraTarget
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param zNear
 * @param zFar
 * @param parent
 */

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

/**
 * @brief Fais pivoter la caméra
 * @param pitch
 * @param yaw
 */

void CameraComponent::rotate( float pitch, float yaw ){

    this->cameraForward.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    this->cameraForward.setY( sin(qDegreesToRadians(pitch)) );
    this->cameraForward.setZ( sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
    this->cameraForward.normalize();
    this->cameraUp =  QVector3D::crossProduct( this->cameraForward, getRight() ).normalized();
}

/**
 * @brief Mets à jour la position de la caméra
 * @param model
 */

void CameraComponent::updatePosition(const QMatrix4x4& model)
{
    this->cameraPosition = model * QVector3D(0.0f,1.75f,2.0f);
}

/**
 * @brief Mets en place une projection perspective
 * @param aspect
 */

void CameraComponent::setProjectionPersp( float aspect )
{
    this->projection.setToIdentity();
    this->projection.perspective( fov, aspect, zNear, zFar );
}

/**
 * @brief Mets en place une projection orthogonale
 */

void CameraComponent::setProjectionOrtho()
{
    this->projection.setToIdentity();
    this->projection.ortho(this->left_O,this->right_O,this->bottom_O,this->top_O,this->zNear,this->zFar);
}

/**
 * @brief Mets en place la cible de la caméra
 * @param cameraTarget
 */

void CameraComponent::setCameraTarget(QVector3D cameraTarget)
{
    this->cameraForward = (cameraTarget - this->cameraPosition).normalized();
}


/**
 * @brief Retourne la matrice de projection
 * @return
 */

const QMatrix4x4 &CameraComponent::getProjection() const
{
    return projection;
}

/**
 * @brief Retourne la matrice de vue
 * @return
 */

const QMatrix4x4& CameraComponent::getViewMatrix(){
    view.setToIdentity();
    view.lookAt(this->cameraPosition, this->cameraPosition + this->cameraForward, this->cameraUp);
    return view;
}


/**
 * @brief Retourne le vecteur Up de la caméra
 * @return
 */

const QVector3D &CameraComponent::getCameraUp() const
{
    return cameraUp;
}


/**
 * @brief Retourne le vecteur Right de la caméra
 * @return
 */

const QVector3D& CameraComponent::getRight(){
    right =  QVector3D::crossProduct( this->yAxis ,this->cameraForward ).normalized();
    return right;
}

/**
 * @brief Retourne le vecteur Left de la caméra
 * @return
 */

const QVector3D& CameraComponent::getLeft(){
    left = QVector3D::crossProduct( this->cameraForward,  this->yAxis ).normalized();
    return left;

}

/**
 * @brief Retourne le vecteur Forward de la caméra
 * @return
 */

const QVector3D& CameraComponent::getCameraForward()
{
    return cameraForward;
}

// Getters & Setters

const QVector3D& CameraComponent::getCameraPosition()
{
    return cameraPosition;
}

const QVector3D& CameraComponent::getCameraTarget()
{
    return cameraTarget;
}

