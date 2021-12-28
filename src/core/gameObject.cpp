#include "headers/core/gameObject.h"


GameObject::GameObject(){}

GameObject::GameObject( std::string name, GameObject * parent ){
    this->name = name;
    this->transform = new Transform( this );
    this->parent = parent;
}

void GameObject::addChild( GameObject *newChildren){
    this->children.push_back( newChildren );
}

void GameObject::move( QVector3D translation ){
    this->transform->applyTranslation( translation );
}

void GameObject::move( float x, float y , float z ){
    this->transform->applyTranslation( QVector3D(x,y,z) );
}


void GameObject::rotate( QVector3D axis, float angle ){
    this->transform->applyRotation( QQuaternion::fromAxisAndAngle( axis, angle ) );

}

void GameObject::rotate( QQuaternion rotation ){
    this->transform->applyRotation(rotation);
}

void GameObject::addRotate( QQuaternion rotation ){
    this->transform->addRotation(rotation);
}


void GameObject::scale( QVector3D scale ){
    this->transform->applyScale( scale );
}

void GameObject::resetModelMatrix(){
    this->transform->resetModel();
}

//Getters & Setters

const std::string &GameObject::getName() const{
    return name;
}

void GameObject::setName(const std::string &newName){
    name = newName;
}

const std::vector<GameObject *> GameObject::getChildren(){
    return this->children;
}

GameObject *GameObject::getParent()
{
    return parent;
}

Transform *GameObject::getTransform()
{
    return this->transform;
}

void GameObject::setTransform( Transform *newTransform)
{
    this->transform = newTransform;
}

QVector3D& GameObject::getWorldPosition(){
    worldPosition = this->transform->getModel() * QVector3D( 0.0, 0.0, 0.0 );
    return worldPosition;
}

const QMatrix4x4 GameObject::getModel(){

    if( !parent ){
        return this->transform->getModel();
    }
    else
         return this->parent->getModel() * this->transform->getModel();


}

