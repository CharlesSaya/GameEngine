#include "headers/core/physicsComponent.h"


PhysicsComponent::PhysicsComponent( PhysicsEngine &physicsEngine,  QObject * parent ) :  physicsEngine(physicsEngine), QObject(parent){
}


void PhysicsComponent::updatePhysics( float step, Transform & transform ){

    rotate();
    transform.applyRotation(this->rotation);

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
    for( uint i : inputsMoves ){
        switch( i ){
            case 0 :
                velocity += rotation *forward  ;
                break;

            case 1 :
                velocity += rotation * backward;
                break;

            case 2 :
                velocity += rotation * left;
                break;

            case 3 :
                velocity += rotation * right;
                break;

            case 4 :
                velocity += jump;
                break;
        }
    }
}

void PhysicsComponent::rotate(){

    rotationAxis[0] += inputsRotationX;
    rotationAxis[1] += inputsRotationY ;
    if(rotationAxis[0]>60) rotationAxis[0] = 60.0f;
    if(rotationAxis[0]<-60) rotationAxis[0] = -60.0f;
//    rotationX =  QQuaternion::fromAxisAndAngle( QVector3D(360.0f,0.0f,0.0f),  rotationAxis[0]);
    rotationY =  QQuaternion::fromAxisAndAngle( QVector3D(0.0f,360.0f,0.0f),  rotationAxis[1]);

    rotation =  rotationX * rotationY   ;

    inputsRotationX = 0.0f;
    inputsRotationY = 0.0f;
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

void PhysicsComponent::hasRotatedX( float inputsRotation ){
    this->inputsRotationX = -inputsRotation;
    rotationAngle+=inputsRotation;
}

void PhysicsComponent::hasRotatedY( float inputsRotation ){
    this->inputsRotationY = -inputsRotation;
    rotationAngle+=inputsRotation;
}

