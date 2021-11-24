#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "headers/render/terrain.h"

#include "headers/core/gameComponent.h"


class MoveComponent : public GameComponent{
    Q_OBJECT

public slots:
    void pressedInput(QKeyEvent * event);
    void releasedInput(QKeyEvent * event);

signals:
    void move( QVector3D movement );
    void stop();

public:

    MoveComponent( float deltaTime, Terrain & terrain );
    void input( QKeyEvent * key  ) override;

    void update( float step ) override;
    void render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) override;

private:
    float speed = .10;
    QVector3D forward  = QVector3D( 0.0   , 0.0 , -speed  );
    QVector3D backward = QVector3D( 0.0   , 0.0 , speed );
    QVector3D right    = QVector3D( speed , 0.0 , 0.0    );
    QVector3D left     = QVector3D( -speed, 0.0 , 0.0    );
    float deltaTime;
    Terrain terrain;
};

#endif // MOVECOMPONENT_H
