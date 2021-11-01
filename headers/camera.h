#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLWidget>

#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class Camera{

private:


public:

    QVector3D yAxis = QVector3D( 0., 1., 0.);

    QVector3D cameraPosition;
    QVector3D cameraTarget;

    QVector3D cameraUp;
    QVector3D cameraForward ;
    QVector3D cameraRight;

    float fov, zNear, zFar;

    Camera(){
    }

    Camera(QVector3D cameraPosition, QVector3D cameraTarget, bool orbital = false){
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;

        this->cameraForward = (this->cameraTarget - this->cameraPosition).normalized();
        this->cameraRight = QVector3D::crossProduct( this->yAxis ,this->cameraForward).normalized();
        this->cameraUp = QVector3D::crossProduct( this->cameraForward, this->cameraRight );

    }

    QMatrix4x4 getViewMatrix(){
        QMatrix4x4 view;

        view.lookAt(this->cameraPosition, this->cameraPosition + this->cameraForward, this->cameraUp);
        return view;
    }

    QVector3D getRight(){
        return QVector3D::crossProduct( this->yAxis ,this->cameraForward ).normalized();
    }

    QVector3D getLeft(){

        return QVector3D::crossProduct( this->cameraForward,  this->yAxis ).normalized();

    }

    void move( QVector3D axis, float movement ){
        this->cameraPosition += axis * movement;
        this->cameraTarget += axis * movement;
    }

    void rotate( float pitch, float yaw ){

        this->cameraForward.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
        this->cameraForward.setY( sin(qDegreesToRadians(pitch)) );
        this->cameraForward.setZ( sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)) );
        this->cameraForward.normalize();
        this->cameraUp =  QVector3D::crossProduct( this->cameraForward, getRight() ).normalized();

    }

};

#endif // CAMERA_H
