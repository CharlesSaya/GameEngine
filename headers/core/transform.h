#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>

class   Transform{

private:
    QVector3D translation;
    QQuaternion rotation;
    QVector3D scale;
    QMatrix4x4 model;
    QVector3D position;
public:
    Transform();
    Transform( QVector3D &position );

    void resetModel();

    void applyTranslation( QVector3D vector );
    void applyRotation( QQuaternion quaternion );
    void applyScale( QVector3D vector );

    void applyRotationAroundParent( QVector3D vector );

    QVector3D &getPosition();
    void setPosition(const QVector3D &newPosition);

    QVector3D getWorldPosition();

    QMatrix4x4 &getModel();

};

#endif // TRANSFORM_H
