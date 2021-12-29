#ifndef CAMERAORTHOCOMPONENT_H
#define CAMERAORTHOCOMPONENT_H

#include <QOpenGLWidget>

#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include "headers/core/transform.h"

class CameraOrthoComponent : public QObject {
    Q_OBJECT

private:
    float fov, zFar, zNear;

    QVector3D yAxis = QVector3D( 0., 1., 0.);

    QVector3D cameraPosition;
    QVector3D cameraTarget;

    QVector3D cameraUp;
    QVector3D cameraForward ;



    QMatrix4x4 view;
    QMatrix4x4 projection;

public:

    CameraOrthoComponent();

    CameraOrthoComponent(QVector3D cameraPosition, QVector3D cameraTarget,float right, float left, float top, float bottom ,
                         float zNear, float zFar, QObject * parent = 0  );


    void updatePosition(const QMatrix4x4& model);

    void move( QVector3D axis );

    void rotate( float pitch, float yaw );
    void setCameraTarget(QVector3D cameraTarget);

    const QMatrix4x4& getViewMatrix();

    const QVector3D& getRight();

    const QVector3D& getLeft();

    const QVector3D& getCameraPosition();
    const QVector3D& getCameraTarget();

    const QVector3D& getCameraForward();

    void setProjection();
    const QMatrix4x4 &getProjection() const;
};



#endif // CAMERAORTHOCOMPONENT_H
