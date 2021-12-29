#include "headers/core/gameObject.h"

GameObject::GameObject(){}

GameObject::GameObject( std::string name, GameObject * parent ){
    this->name = name;
    this->transform = new Transform( this );
    this->lastParent = parent;
    this->parent = parent;
}

void GameObject::addChild( GameObject *newChildren ){
    this->children.push_back( newChildren );
}

void GameObject::removeChild( GameObject *go ){
    std::vector<GameObject *>::iterator itA;
    itA = find( children.begin(), children.end(), go);
    if ( itA != children.end()  )
        children.erase( itA );
    else
        qDebug() << "not";
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

void GameObject::resetTransform(){
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

void GameObject::setParent(GameObject *newParent){

    qDebug() << "sphere world position" << this->getWorldPosition();
    qDebug() << "parent" << newParent->getWorldPosition();
    qDebug() << "sphere - parent" <<  ( getWorldPosition() - newParent->getWorldPosition() );

    QVector3D trans = ( getWorldPosition() - newParent->getWorldPosition() );

    this->transform->setPosition( trans );

    qDebug() << "sphere world position" << getWorldPosition();

    newParent->addChild(this);
    parent->removeChild( this );
    lastParent = parent;
    parent = newParent;

}

void GameObject::setLastParent(){

    qDebug() << "sphere relative to bunny" << this->getWorldPosition();
    qDebug() << "Direction Bunny -> Sphere" <<  ( getWorldPosition() - parent->getWorldPosition() );

    qDebug() << "sphere relative after translate " << this->getWorldPosition();

    QVector3D trans = ( getWorldPosition() - lastParent->getWorldPosition() );

    this->transform->setPosition( trans );
    qDebug() << "sphere world position" << getWorldPosition();

    parent->removeChild( this );
    lastParent->addChild( this );
    parent = lastParent;

    lastParent = nullptr;

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
    worldPosition = this->getModel() * QVector3D( 0.0, 0.0, 0.0 );
    return worldPosition;
}

const QMatrix4x4 GameObject::getModel(){

    if( !parent ){
        return this->transform->getModel();
    }
    else
         return this->parent->getModel() * this->transform->getModel();


}

