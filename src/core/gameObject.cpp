#include "headers/core/gameObject.h"


GameObject::GameObject(){}

GameObject::GameObject( std::string name, GameObject * parent ){
    this->name = name;
    this->transform = new Transform( this );
    this->lastParent = parent;
    this->parent = parent;
    parent->addChild(this);
}

void GameObject::addChild( GameObject *newChildren ){
    this->children.push_back( newChildren );
}

void GameObject::removeChild( GameObject *go ){
    std::vector<GameObject *>::iterator itA;
    itA = find( children.begin(), children.end(), go);
    if ( itA != children.end()  )
        children.erase( itA );

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

void GameObject::scale( QVector3D scale ){
    this->transform->applyScale( scale );
}

void GameObject::scale( float scale ){
    this->transform->applyScale(scale);

}

void GameObject::destroy(){
    isDestroyed = true;
    this->parent->removeChild( this );
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

    QVector3D trans = getWorldPosition();
    newParent->addChild(this);
    parent->removeChild( this );
    lastParent = parent;
    parent = newParent;
    QVector3D scaleParent = parent->getTransform()->getScale();
    predScale = this->getTransform()->getScale();
    QVector3D invertScale = this->getTransform()->getScale() * QVector3D(1/scaleParent.x(),1/scaleParent.y(), 1/scaleParent.z());
    getTransform()->applyScale(invertScale);
    QVector3D invertTrans =  newParent->getModel().inverted()* trans  ;
    transform->setPosition(invertTrans);
    setIsTelekinesis(true);


}

void GameObject::setLastParent(){
    QVector3D trans = getWorldPosition() ;
    parent->removeChild( this );
    lastParent->addChild( this );
    parent = lastParent;
    QVector3D invertTrans = parent->getModel().inverted() * trans ;
    getTransform()->applyScale(predScale);
    transform->setPosition( invertTrans );
    lastParent = nullptr;
    setIsTelekinesis(false);
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

void GameObject::setHeight(float height){
    this->transform->getPosition().setY( height );
}

bool GameObject::getIsTelekinesis() const
{
    return isTelekinesis;
}

void GameObject::setIsTelekinesis(bool newIsTelekinesis)
{
    isTelekinesis = newIsTelekinesis;
}


const QMatrix4x4 GameObject::getModel(){

    if( !parent ){
        return this->transform->getModel();
    }
    else
        return this->parent->getModel() * this->transform->getModel();

}


bool GameObject::getIsMovable() const
{
    return isMovable;
}

void GameObject::setIsMovable(bool newIsMovable)
{
    isMovable = newIsMovable;
}

bool GameObject::getIsCollectible() const
{
    return isCollectible;
}

void GameObject::setIsCollectible(bool newIsCollectible)
{
    isCollectible = newIsCollectible;
}

bool GameObject::getIsDestroyed() const
{
    return isDestroyed;
}

void GameObject::setIsDestroyed(bool newIsDestroyed)
{
    isDestroyed = newIsDestroyed;
}
