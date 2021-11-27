#include "headers/physics/physicsEngine.h"

PhysicsEngine::PhysicsEngine(  ){

}

PhysicsEngine::PhysicsEngine( float deltaTime ){

    this->deltaTime = deltaTime;
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

