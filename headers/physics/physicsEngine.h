#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "headers/render/AABB.h"

class PhysicsEngine
{
public:
    PhysicsEngine();
    PhysicsEngine( float deltaTime );
    void computeForces();
    void update(  );
    bool intersectAABB( AABB &box1, AABB &box2 );
    const QVector3D &getGravity() const;
    void setGravity(const QVector3D &newGravity);

    float getDeltaTime() const;
    void setDeltaTime(float newDeltaTime);

    float getDamp() const;
    void setDamp(float newDamp);

    float getWind() const;
    void setWind(float newWind);

    float getFriction() const;
    void setFriction(float newFriction);

private:

    QVector3D gravity = QVector3D(0., -30.81, 0.);

    float deltaTime = 0.0f;
    float damp      = .5f;
    float wind      = 1.0f;
    float friction  = 1.0f;

};

#endif // PHYSICSENGINE_H
