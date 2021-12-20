#include "headers/core/physicsComponent.h"


PhysicsComponent::PhysicsComponent( PhysicsEngine &physicsEngine,  QObject * parent ) :  physicsEngine(physicsEngine), QObject(parent){
}


void PhysicsComponent::updatePhysics( float step, Transform & transform ){

    move();

    acceleration = - physicsEngine.getDamp() * velocity;

    acceleration += physicsEngine.getGravity();

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


bool PhysicsComponent::restin( QVector3D& normal, float distance ){
    QVector3D vn = QVector3D::dotProduct( velocity, normal ) * normal;
    QVector3D vt = velocity  - vn;

    if (this->velocity.length() < 0.5 && distance < 0.01 && QVector3D::dotProduct( acceleration, normal ) < 0.001 && vt.length() < this->friction * vn.length()  )
        return true;

    return false;
}

void PhysicsComponent::move(){
    for( uint i : inputs ){
        switch( i ){
            case 0 :
                velocity += forward;
                break;

            case 1 :
                velocity += backward;
                break;

            case 2 :
                velocity += left;
                break;

            case 3 :
                velocity += right;
                break;

            case 4 :
                velocity += jump;
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

void PhysicsComponent::hasMoved( QSet<uint> inputs ){
    this->inputs = inputs;
    resting = false;
}

void PhysicsComponent::hasStopped( QSet<uint> inputs ){
    this->inputs = inputs;
    stop();
}


