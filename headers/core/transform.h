#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>

class Transform : public QObject{

Q_OBJECT

signals:
    void transformed();

private:
    QQuaternion rotation;
    QVector3D scale;
    QMatrix4x4 model;
    QVector3D position;
    QVector3D lastWorldPosition;

public:
    Transform( QObject * parent );
    Transform( Transform & transform );
    Transform( QVector3D &position );

    void resetModel();

    void applyTranslation( QVector3D vector );
    void applyRotation( QQuaternion quaternion );
    void applyScale( QVector3D vector );

    QVector3D &getPosition();
    void setPosition(const QVector3D &newPosition);

    void setLastWorldPosition();
    QVector3D getWorldPosition();

    QMatrix4x4 &getModel();

};

#endif // TRANSFORM_H
