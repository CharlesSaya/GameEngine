#include "headers/core/gameObject.h"

GameObject::GameObject(){}

GameObject::GameObject( std::string name, GameObject * parent){
    this->name = name;
    this->transform = Transform();
    this->parent = parent;
    this->children = std::vector<GameObject *> ();
}

GameObject::GameObject( std::string name, std::string filename, QVector3D meshColor, GameObject * parent){
    this->name = name;
    this->transform = Transform();
    this->parent = parent;
    this->mesh = Mesh( filename, meshColor );
    this->containsMesh = true;
}

const std::string &GameObject::getName() const
{
    return name;
}

void GameObject::setName(const std::string &newName)
{
    name = newName;
}


void GameObject::addChild( GameObject *newChildren)
{
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

void GameObject::setMesh( Mesh &mesh ){
    this->mesh = mesh;
}

Mesh &GameObject::getMesh(){
    return this->mesh;
}

bool GameObject::hasMesh(){
    return this->containsMesh;
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

void GameObject::drawMesh( QOpenGLShaderProgram * program  ){

    this->mesh.drawMesh( program );
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

void GameObject::input(){
    Q_FOREACH( GameComponent * component, gameComponents )
        component->input();
}

void GameObject::update(){
    Q_FOREACH( GameComponent * component, gameComponents )
        component->update();
}

void GameObject::render(){
    Q_FOREACH( GameComponent * component, gameComponents )
        component->render();
}
