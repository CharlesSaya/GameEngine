#include "headers/core/physicsComponent.h"


PhysicsComponent::PhysicsComponent( Transform &transform, PhysicsEngine &physicsEngine ): transform(transform){
    this->physicsEngine = physicsEngine;
}

void PhysicsComponent::input( QKeyEvent * key  ){

}

void PhysicsComponent::update( float step ){


    QVector3D acceleration = (- physicsEngine.getDamp() * velocity) / mass;

    QVector3D newVelocity  = velocity + acceleration * step;

    QVector3D meanSpeed = (velocity + newVelocity) / 2.0 ;
    if( meanSpeed.length() > this->maxSpeedWalk )
        meanSpeed = this->maxSpeedWalk * meanSpeed.normalized();

    this->transform.applyTranslation(meanSpeed * step);
    velocity = newVelocity;

}

void PhysicsComponent::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ){


}


void PhysicsComponent::hasMoved( QVector3D movement ){
    velocity += movement;
}

void PhysicsComponent::hasStopped(){
    velocity = QVector3D();
}


