#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>

class Transform : public QObject{

Q_OBJECT

signals:
    void transformed();

private:
    QQuaternion rotation;
    QQuaternion defaultRotation;
    QVector3D scale;
    QMatrix4x4 model;
    QVector3D position;
    QVector3D lastWorldPosition;
    QVector3D worldPosition;

public:
    Transform( QObject * parent );
    Transform( Transform & transform );
    Transform( QVector3D &position );

    void resetModel();

    void applyTranslation( QVector3D vector );
    void applyRotation( QQuaternion quaternion );
    void applyScale( QVector3D vector );
    void applyScale( float scale );

    QVector3D &getPosition();
    QQuaternion &getRotation();
    QVector3D &getScale();
    void setPosition(const QVector3D &newPosition);

    void setLastWorldPosition();
    QVector3D& getWorldPosition();

    QMatrix4x4 &getModel();

};

#endif // TRANSFORM_H
