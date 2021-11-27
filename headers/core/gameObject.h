#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <QVector3D>

#include "headers/core/gameComponent.h"
#include "headers/core/meshrenderer.h"

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

    void addChild( GameObject * newChildren );
    void addComponent( GameComponent * component );
    void removeComponent( GameComponent * component);

    void input( QKeyEvent * key );
    void update( float step );
    void render( const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition );

    void move( QVector3D translation );
    void rotate( QVector3D axis, float angle );
    void scale( QVector3D scale );

    void resetModelMatrix();

    const std::string &getName() const;
    void setName(const std::string &newName);

    void setTransform(const Transform &newTransform);
    Transform &getTransform();

    QMatrix4x4 getModel();
    QVector3D getWorldPosition();
    const std::vector<GameObject *> getChildren();


    template<typename T> GameComponent * getComponent(){

        for( GameComponent * go : this->gameComponents ){
            if( instanceof<T>( go ) )
                return go;
        }

        return nullptr;
    }

};




#endif // GAMEOBJECT_H
