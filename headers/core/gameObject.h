#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <QVector3D>

#include "headers/core/gameComponent.h"

#include "headers/render/shader.h"

#include "transform.h"

class GameObject{

private:

    std::string name;
    Transform transform;
    GameObject * parent;

    std::vector<GameObject *> children;
    std::vector<GameComponent * > gameComponents;

public:
    GameObject();
    GameObject( std::string name, GameObject * parent = nullptr );

    void setFaces( const std::vector< std::vector< int > > &faces );
    const std::vector< std::vector< int > > getFaces();

    void addChild( GameObject * newChildren );
    const std::vector<GameObject *> getChildren();

    void setTransform(const Transform &newTransform);
    Transform &getTransform();

    QVector3D getWorldPosition();

    void addComponent( GameComponent * component );
    void removeComponent( GameComponent * component);

    void input();
    void update();
    void render( const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition );

    void move( QVector3D translation );
    void rotate( QVector3D axis, float angle );
    void scale( QVector3D scale );

    void applyTransformation( Transform &transform );

    QMatrix4x4 getModel();

    const std::string &getName() const;
    void setName(const std::string &newName);

    void resetModelMatrix();

};


#endif // GAMEOBJECT_H
