#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>

class Transform{

private:
    QVector3D translation;
    QQuaternion rotation;
    QVector3D scale;
    QMatrix4x4 model;

public:
    Transform();
    Transform( QVector3D &translation,  QQuaternion & rotation, QVector3D  &scale );

    void resetModel();
    QMatrix4x4 & getModel();

    void applyTranslation( QVector3D vector );
    void applyRotation( QQuaternion quaternion );
    void applyScale( QVector3D vector );

    void applyRotationAroundParent( QVector3D vector );

};

#endif // TRANSFORM_H
