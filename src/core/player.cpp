#include "headers/core/player.h"

Player::Player(){

}

Player::Player( GameObject & gameObject ){
    this->player = &gameObject;
}

void Player::move( QVector3D translation,  Terrain & terrain ){
    this->player->move( translation );
    QVector3D currentPos =  this->player->getWorldPosition() ;
    this->setHeight( terrain.getHeightOfTerrain( currentPos ) );
}


void Player::setHeight( float height ){
    this->player->getTransform()->getPosition().setY( height + this->mesh.getAABB().getHeight()  / 2.);
}

void Player::setMesh(const Mesh &newMesh)
{
    mesh = newMesh;
}
