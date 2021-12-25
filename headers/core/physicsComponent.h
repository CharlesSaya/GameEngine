#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "headers/core/transform.h"
#include "headers/core/gameComponent.h"
#include "headers/physics/physicsEngine.h"


class PhysicsComponent : public QObject{

    Q_OBJECT

public slots:

    void hasMoved( QSet<uint> inputs );
    void hasStopped( QSet<uint> inputs );
    void hasRotatedX( float inputs );
    void hasRotatedY( float inputs );

public:

    PhysicsComponent(  PhysicsEngine &physicsEngine, QObject * parent = 0 );

    void updatePhysics( float step, Transform & transform );
    void stop();
    PhysicsEngine &physicsEngine;

    void move();
    void rotate();

    bool atRest();
    bool restin( QVector3D& normal, float distance );
    // Getters & Setters

    const QVector3D &getVelocity() const;
    void setVelocity(const QVector3D &newVelocity);

    float getRestitution() const;
    void setRestitution(float newRestitution);

    float getFriction() const;
    void setFriction(float newFriction);

    const QVector3D &getAcceleration() const;
    void setAcceleration(const QVector3D &newAcceleration);

    bool getResting() const;
    void setResting(bool newResting);

private:
    float speed = 0.15;

    float mass = 1.0;
    float friction = 1.0;
    float restitution = .0;
    float maxSpeedWalk = 5.0;

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
    QVector3D jump     = QVector3D( 0.0, speed , 0.0    );

    QQuaternion rotation;
    QQuaternion rotationX;
    QQuaternion rotationY;
    QVector3D rotationAxis;
    float rotationAngle;

};

#endif // PHYSICSCOMPONENT_H
