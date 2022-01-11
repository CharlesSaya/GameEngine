#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "headers/core/transform.h"
#include "headers/core/gameComponent.h"
#include "headers/physics/physicsEngine.h"
#include "headers/core/gameObject.h"

#include "headers/render/terrain.h"


class PhysicsComponent : public QObject{

    Q_OBJECT

public slots:

    void hasMoved( QSet<uint> inputs );
    void hasStopped( QSet<uint> inputs );

public:

    PhysicsComponent(  PhysicsEngine &physicsEngine, QObject * parent = 0 );

    void updatePhysics( float step, GameObject * go, Terrain & terrain );
    void updatePhysicsMesh( float step, GameObject * go );
    void stop();
    PhysicsEngine &physicsEngine;

    void move(Transform & transform);
    void rotate();

    bool atRest();
    // Getters & Setters

    const QVector3D &getVelocity() const;
    void setVelocity(const QVector3D newVelocity);

    float getRestitution() const;
    void setRestitution(float newRestitution);

    float getFriction() const;
    void setFriction(float newFriction);

    const QVector3D &getAcceleration() const;
    void setAcceleration(const QVector3D &newAcceleration);

    bool getResting() const;
    void setResting(bool newResting);

private:

    float speed = 1.0f    ;
    float mass = 1.0;
    float friction = 1.0;
    float restitution = .00;
    float maxSpeed = 2.0;

    bool resting = false;

    QSet<uint> inputsMoves;

    float inputsRotationX;
    float inputsRotationY;

    QVector3D acceleration = QVector3D( 0.0, 0.0, 0.0 );
    QVector3D velocity     = QVector3D( 0.0, 0.0, 0.0 );

    QVector3D forward  = QVector3D( 0.0   , 0.0 , -speed  );
    QVector3D backward = QVector3D( 0.0   , 0.0 , speed );
    QVector3D right    = QVector3D( speed , 0.0 , 0.0    );
    QVector3D left     = QVector3D( -speed, 0.0 , 0.0    );
    QVector3D jump     = QVector3D( 0.0, 10000. * speed , 0.0    );
    QVector3D dive     = QVector3D( 0.0, -speed , 0.0    );

};

#endif // PHYSICSCOMPONENT_H
