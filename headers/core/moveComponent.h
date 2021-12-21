#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "headers/render/terrain.h"

#include "headers/core/gameComponent.h"


class MoveComponent : public QObject{
    Q_OBJECT

public slots:

    void pressedInput(QKeyEvent * event);
    void releasedInput(QKeyEvent * event);

signals:
    void move( QSet<uint> inputs );
    void stop( QSet<uint> inputs );

public:

    MoveComponent( Terrain & terrain );

    void input( QKeyEvent * key  );
    void update( float step );

private:
    float speed = 1.;


    QVector3D forward  = QVector3D( 0.0   , 0.0 , -speed  );
    QVector3D backward = QVector3D( 0.0   , 0.0 , speed );
    QVector3D right    = QVector3D( speed , 0.0 , 0.0    );
    QVector3D left     = QVector3D( -speed, 0.0 , 0.0    );
    QVector3D jump     = QVector3D( 0.0, speed , 0.0    );

    QSet<uint> inputs;

    Terrain terrain;
};

#endif // MOVECOMPONENT_H
