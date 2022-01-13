#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "headers/render/terrain.h"

#include "headers/core/gameComponent.h"
#include "headers/core/transform.h"


class MoveComponent : public QObject{
    Q_OBJECT

public slots:

    void pressedInput(QKeyEvent * event);
    void releasedInput(QKeyEvent * event);
    void mouseMoveEvent(QVector2D pos, bool resetX, bool resetY );

signals:
    void move( QSet<uint> inputs );
    void stop( QSet<uint> inputs );

    void moveChild( QSet<uint> inputs );
    void stopChild( QSet<uint> inputs );

public:

    MoveComponent( Terrain & terrain, QObject * parent = 0  );

    void input( QKeyEvent * key  );

    QQuaternion getRotationY();
    QQuaternion getRotationX();

private:
    float speed = 1.;

    float yaw ;
    float pitch;
    QQuaternion rotationX = QQuaternion();
    QQuaternion rotationY = QQuaternion();
    QVector3D rotationAxis;

    QVector2D mousePressPosition;

    QVector3D forward  = QVector3D( 0.0   , 0.0 , -speed  );
    QVector3D backward = QVector3D( 0.0   , 0.0 , speed );
    QVector3D right    = QVector3D( speed , 0.0 , 0.0    );
    QVector3D left     = QVector3D( -speed, 0.0 , 0.0    );
    QVector3D jump     = QVector3D( 0.0, speed , 0.0    );

    QSet<uint> inputs;
    QSet<uint> inputsChild;
    float inputsX;

    Terrain terrain;
};

#endif // MOVECOMPONENT_H
