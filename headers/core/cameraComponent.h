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

public slots:


protected:
    bool eventFilter( QObject * obj, QEvent * event );

private:
    float fov, zFar, zNear;

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

    CameraComponent(QVector3D cameraPosition, QVector3D cameraTarget, float fov, float zNear, float zFar );

    void updatePosition(Transform & transform);

    void move( QVector3D axis );

    void rotate( float pitch, float yaw );

    const QMatrix4x4& getViewMatrix();

    const QVector3D& getRight();

    const QVector3D& getLeft();

    const QVector3D& getCameraPosition();

    const QVector3D& getCameraForward();

    void setProjection(float aspect);
    const QMatrix4x4 &getProjection() const;
};


#endif // CAMERACOMPONENT_H