#include "headers/core/physicsComponent.h"


PhysicsComponent::PhysicsComponent( PhysicsEngine &physicsEngine,  QObject * parent ) :  physicsEngine(physicsEngine), QObject(parent){
}


void PhysicsComponent::updatePhysics( float step, Transform & transform ){

    move(transform);
    acceleration = - physicsEngine.getDamp() * velocity;

    if ( !resting )
//        acceleration += physicsEngine.getGravity();

    acceleration /= mass;

    QVector3D newVelocity  = velocity + acceleration * step;

    QVector3D meanSpeed = (velocity + newVelocity) / 2.0 ;


    if( meanSpeed.length() > this->maxSpeedWalk )
        meanSpeed = this->maxSpeedWalk * meanSpeed.normalized();

    if ( meanSpeed.length() != 0.0f ){
        transform.applyTranslation( meanSpeed * step );
    }
    velocity = newVelocity;

}

void PhysicsComponent::move(Transform & transform){
    for( uint i : inputsMoves ){
        switch( i ){
            case 0 :
                velocity += transform.getRotation() * forward  ;
                break;

            case 1 :
                velocity += transform.getRotation() *  backward;
                break;

            case 2 :
                velocity += transform.getRotation() * left;
                break;

            case 3 :
                velocity += transform.getRotation() * right;
                break;

            case 4 :
                velocity += transform.getRotation() * jump;
                break;
        }
    }
}

bool PhysicsComponent::atRest(){
    return resting;
}

void PhysicsComponent::stop()
{
    velocity = QVector3D();
}

// Getters & Setters

const QVector3D &PhysicsComponent::getAcceleration() const
{
    return acceleration;
}

void PhysicsComponent::setAcceleration(const QVector3D &newAcceleration)
{
    acceleration = newAcceleration;
}

bool PhysicsComponent::getResting() const
{
    return resting;
}

void PhysicsComponent::setResting(bool newResting)
{
    resting = newResting;
}

const QVector3D &PhysicsComponent::getVelocity() const
{
    return velocity;
}

void PhysicsComponent::setVelocity(const QVector3D &newVelocity)
{
    velocity = newVelocity;
}

float PhysicsComponent::getRestitution() const
{
    return restitution;
}

void PhysicsComponent::setRestitution(float newRestitution)
{
    restitution = newRestitution;
}

float PhysicsComponent::getFriction() const
{
    return friction;
}

void PhysicsComponent::setFriction(float newFriction)
{
    friction = newFriction;
}

void PhysicsComponent::hasMoved( QSet<uint> inputsMoves ){
    this->inputsMoves = inputsMoves;
    resting = false;
}

void PhysicsComponent::hasStopped( QSet<uint> inputsMoves ){
    this->inputsMoves = inputsMoves;
    stop();
}
