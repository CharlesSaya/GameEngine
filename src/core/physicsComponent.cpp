#include "headers/core/physicsComponent.h"


PhysicsComponent::PhysicsComponent( PhysicsEngine &physicsEngine,  QObject * parent ) :  physicsEngine(physicsEngine), QObject(parent){
}



void PhysicsComponent::updatePhysics( float step, Transform & transform ){

    QVector3D acceleration = ( physicsEngine.getGravity() - physicsEngine.getDamp() * velocity) / mass;

    QVector3D newVelocity  = velocity + acceleration * step;

    QVector3D meanSpeed = (velocity + newVelocity) / 2.0 ;

    if( meanSpeed.length() > this->maxSpeedWalk )
        meanSpeed = this->maxSpeedWalk * meanSpeed.normalized();

    if ( meanSpeed.length() != 0.0f ){
        transform.applyTranslation( meanSpeed * step );
    }

    velocity = newVelocity;

}

void PhysicsComponent::stop(){
    velocity = QVector3D();

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

void PhysicsComponent::hasMoved( QVector3D movement ){
    velocity += movement;
}

void PhysicsComponent::hasStopped(){
    stop();
}


