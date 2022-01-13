#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include <QOpenGLWidget>

#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include "headers/core/transform.h"

class CameraComponent : public QObject {
    Q_OBJECT


private:
    float fov, zFar, zNear;

    //Orthogonal
    float right_O;
    float left_O;
    float top_O;
    float bottom_O;

    QVector3D yAxis = QVector3D( 0., 1., 0.);

    QVector3D cameraPosition;
    QVector3D cameraTarget;

    QVector3D cameraUp;
    QVector3D cameraForward ;
    QVector3D right;
    QVector3D left;

    QMatrix4x4 view;
    QMatrix4x4 projection;

public:

    CameraComponent();

    CameraComponent(QVector3D cameraPosition, QVector3D cameraTarget, float fov, float zNear, float zFar, QObject * parent = 0  );
    CameraComponent(QVector3D cameraPosition, QVector3D cameraTarget,float left, float right, float bottom, float top ,
                    float zNear, float zFar, QObject * parent = 0  );

    void updatePosition(const QMatrix4x4& model);

    void rotate( float pitch, float yaw );
    void setCameraTarget(QVector3D cameraTarget);

    const QMatrix4x4& getViewMatrix();

    const QVector3D& getRight();

    const QVector3D& getLeft();

    const QVector3D& getCameraPosition();
    const QVector3D& getCameraTarget();

    const QVector3D& getCameraForward();

    void setProjectionPersp(float aspect);
    void setProjectionOrtho();
    const QMatrix4x4 &getProjection() const;
    const QVector3D &getCameraUp() const;
};


#endif // CAMERACOMPONENT_H
