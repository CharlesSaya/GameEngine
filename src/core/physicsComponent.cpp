#include "headers/core/physicsComponent.h"


PhysicsComponent::PhysicsComponent( PhysicsEngine &physicsEngine,  QObject * parent ) :  physicsEngine(physicsEngine), QObject(parent){
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
 * @brief Mets à jour la physique ( vitesse, position  du joueur
 * @param step
 * @param go
 * @param terrain
 */

void PhysicsComponent::updatePhysics( float step,  GameObject * go, Terrain &terrain ){

    worldPos= go->getWorldPosition();
    height = terrain.getHeightTerrain( worldPos );

    if(playerIsOnGround()&& !(height > worldPos.y() && acos( QVector3D::dotProduct( terrain.getFaceNormalAtPosition( worldPos ), QVector3D( 0.0, 1.0, 0.0) ) ) * 180 / M_PI > 60.0) || getResting() ) canJump=true;
    else canJump =false;

    move(*go->getTransform());
    acceleration = - physicsEngine.getDamp() * velocity;

    if ( !resting )
        acceleration += physicsEngine.getGravity();

    acceleration /= mass;

    QVector3D newVelocity  = velocity + acceleration * step;

    QVector3D meanSpeed = (velocity + newVelocity) / 2.0 ;

//    if( abs( meanSpeed.x() )> this->maxSpeed )
//        meanSpeed.setX( sgn( meanSpeed.x() ) * this->maxSpeed   );

//    if( abs( meanSpeed.y() )> 2 * this->maxSpeed )
//        meanSpeed.setY( sgn( meanSpeed.y() ) * 2 * this->maxSpeed  );

//    if( abs( meanSpeed.z() )> this->maxSpeed )
//        meanSpeed.setZ( sgn( meanSpeed.z() ) * this->maxSpeed );

    if( meanSpeed.length()  > this->maxSpeed )
        meanSpeed = this->maxSpeed * meanSpeed.normalized();
    meanSpeed.setY( meanSpeed.y() * 1.2f );
    if ( meanSpeed.length() != 0.0f ){
        go->move( meanSpeed * step );
        worldPos= go->getWorldPosition();
        height = terrain.getHeightTerrain( worldPos );
        QVector3D normal =  terrain.getFaceNormalAtPosition( worldPos );
        if( height > worldPos.y() && acos( QVector3D::dotProduct( normal, QVector3D( 0.0, 1.0, 0.0) ) ) * 180 / M_PI > 60.0 )
            go->move( - meanSpeed * step );
    }

    velocity = meanSpeed;


}

/**
 * @brief Mets à jour la physique ( vitesse, position ) d'un Mesh si nécessaire
 * @param step
 * @param go
 */

void PhysicsComponent::updatePhysicsMesh( float step,  GameObject * go ){

    acceleration = - physicsEngine.getDamp() * velocity;

    if ( !resting )
        acceleration += physicsEngine.getGravity();

    acceleration /= mass;

    QVector3D newVelocity  = velocity + acceleration * step;

    QVector3D meanSpeed = (velocity + newVelocity) / 2.0 ;

    if( meanSpeed.length() > this->maxSpeed )
        meanSpeed = this->maxSpeed * meanSpeed.normalized();

    if ( meanSpeed.length() != 0.0f ){
        go->move( meanSpeed * step );
    }
    velocity = newVelocity;

}

/**
 * @brief Mets à jour la vélocité actuelle en fonction des inputs du joueur
 * @param transform
 */

void PhysicsComponent::move(Transform & transform){
    for( uint i : inputsMoves ){
        switch( i ){
            case 0 :
                velocity += transform.getRotation() * forward;
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

                if(canJump){
                    velocity.setY(0.1f);
                    velocity +=  jump ;
                    canJump = false;
                    if(!playedMusic){
                        SoundEngine().jump();
                        playedMusic = true;
                    }

                }
                break;

            case 5 :
                velocity += dive;
                break;
        }
    }
}

/**
 * @brief Prédicat pour récupérer l'état de repos d'un joueur
 * @return
 */

bool PhysicsComponent::atRest(){
    return resting;
}

/**
 * @brief Prédicat pour récupérer l'état grounded d'un joueur
 * @return
 */

bool PhysicsComponent::playerIsOnGround(){
     if(worldPos.y() - height < 0.3 )
             return true;
     else return false;
}

/**
 * @brief Mets à nul la vitesse du joueur
 */

void PhysicsComponent::stop()
{
    if(velocity.y()<0.0) velocity = QVector3D(0.0f,velocity.y(),0.0f);
    else
    velocity = QVector3D();

    playedMusic = false;
}


// Signals & Slots

/**
 * @brief Slot activé lorsque que le joueur appuie sur une touche de mouvement
 * @param inputsMoves
 */

void PhysicsComponent::hasMoved( QSet<uint> inputsMoves ){
    this->inputsMoves = inputsMoves;
//    resting = false;
}

/**
 * @brief Slot activé lorsque que le joueur relâche une touche de mouvement
 * @param inputsMoves
 */

void PhysicsComponent::hasStopped( QSet<uint> inputsMoves ){
    this->inputsMoves = inputsMoves;
    stop();
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

void PhysicsComponent::setVelocity(const QVector3D newVelocity)
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


