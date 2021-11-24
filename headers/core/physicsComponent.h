#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "headers/core/transform.h"
#include "headers/core/gameComponent.h"
#include "headers/physics/physicsEngine.h"


class PhysicsComponent : public GameComponent{
Q_OBJECT

public slots:

    void hasMoved( QVector3D movement );
    void hasStopped( );

public:

    PhysicsComponent( Transform &transform, PhysicsEngine &physicsEngine );

    void input( QKeyEvent * key  ) override;
    void update( float step ) override;
    void render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) override;

    virtual ~PhysicsComponent();

private:

    float mass = 1.0;
    float friction = 1.0;
    float movementSpeed = 0.1;
    float maxSpeedWalk = 1.0;
    PhysicsEngine physicsEngine;

    QVector3D velocity = QVector3D( 0.0, 0.0, 0.0 );

    Transform &transform;
};

#endif // PHYSICSCOMPONENT_H
