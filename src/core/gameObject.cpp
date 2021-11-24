#include "headers/core/gameObject.h"


GameObject::GameObject(){}

GameObject::GameObject( std::string name, GameObject * parent ){
    this->name = name;
    this->transform = Transform();
    this->parent = parent;
}


const std::string &GameObject::getName() const{
    return name;
}

void GameObject::setName(const std::string &newName){
    name = newName;
}

void GameObject::addChild( GameObject *newChildren){
    this->children.push_back( newChildren );
}

const std::vector<GameObject *> GameObject::getChildren(){
    return this->children;
}

Transform &GameObject::getTransform()
{
    return this->transform;
}

void GameObject::setTransform(const Transform &newTransform)
{
    this->transform = newTransform;
}

QVector3D GameObject::getWorldPosition(){
    return this->transform.getWorldPosition();
}

void GameObject::move( QVector3D translation ){
    this->transform.applyTranslation( translation );
}

void GameObject::rotate( QVector3D axis, float angle ){
    this->transform.applyRotation( QQuaternion::fromAxisAndAngle( axis, angle ) );
}

void GameObject::scale( QVector3D scale ){
    this->transform.applyScale( scale );
}

void GameObject::resetModelMatrix(){
    this->transform.resetModel();
}

QMatrix4x4 GameObject::getModel(){
    if( !parent ){
        return this->transform.getModel();
    }
    else
        return this->parent->getModel() * this->transform.getModel();
}

void GameObject::addComponent(GameComponent *component){
    this->gameComponents.push_back( component );
}

void GameObject::input( QKeyEvent * key ){
    for( GameComponent * component : gameComponents )
        component->input(  key );

    for( GameObject * child : this->children )
        child->input( key );
}

void GameObject::update( float step ){
    for( GameComponent * component : gameComponents )
        component->update( step );

    for( GameObject * child : this->children )
        child->update( step );

}

void GameObject::render( const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition ){

    for( GameComponent * component : gameComponents )
        component->render( model, view, projection, cameraPosition );

    for( GameObject * child : this->children )
        child->render( child->getModel(), view, projection, cameraPosition );
}
