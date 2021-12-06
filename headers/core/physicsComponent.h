#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "headers/core/transform.h"
#include "headers/core/gameComponent.h"
#include "headers/physics/physicsEngine.h"


class PhysicsComponent : public QObject{

    Q_OBJECT

public slots:

    void hasMoved( QVector3D movement );
    void hasStopped( );

public:

    PhysicsComponent(  PhysicsEngine &physicsEngine, QObject * parent = 0 );

    void updatePhysics( float step, Transform & transform );
    void stop();
    PhysicsEngine &physicsEngine;

    const QVector3D &getVelocity() const;
    void setVelocity(const QVector3D &newVelocity);

    float getRestitution() const;
    void setRestitution(float newRestitution);

    float getFriction() const;
    void setFriction(float newFriction);

private:

    float mass = 1.0;
    float friction = 1.0;
    float restitution = .3;
    float maxSpeedWalk = 1.0;

    QVector3D velocity = QVector3D( 0.0, 0.0, 0.0 );

};

#endif // PHYSICSCOMPONENT_H
