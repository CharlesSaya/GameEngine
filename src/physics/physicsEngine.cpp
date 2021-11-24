#include "headers/physics/physicsEngine.h"

PhysicsEngine::PhysicsEngine(  ){

}

PhysicsEngine::PhysicsEngine( float deltaTime ){

    this->deltaTime = deltaTime;
}


bool PhysicsEngine::intersectAABB( AABB &box1, AABB &box2 ){
    return (  ( ( box1.getMin().x() < box2.getMax().x() ) && ( box1.getMax().x() > box2.getMin().x() ) )
           || ( ( box1.getMin().y() < box2.getMax().y() ) && ( box1.getMax().y() > box2.getMin().y() ) )
              || ( ( box1.getMin().z() < box2.getMax().z() ) && ( box1.getMax().z() > box2.getMin().z() ) ) );
}

const QVector3D &PhysicsEngine::getGravity() const
{
    return gravity;
}

void PhysicsEngine::setGravity(const QVector3D &newGravity)
{
    gravity = newGravity;
}

float PhysicsEngine::getDeltaTime() const
{
    return deltaTime;
}

void PhysicsEngine::setDeltaTime(float newDeltaTime)
{
    deltaTime = newDeltaTime;
}

float PhysicsEngine::getDamp() const
{
    return damp;
}

void PhysicsEngine::setDamp(float newDamp)
{
    damp = newDamp;
}

float PhysicsEngine::getWind() const
{
    return wind;
}

void PhysicsEngine::setWind(float newWind)
{
    wind = newWind;
}

float PhysicsEngine::getFriction() const
{
    return friction;
}

void PhysicsEngine::setFriction(float newFriction)
{
    friction = newFriction;
}

