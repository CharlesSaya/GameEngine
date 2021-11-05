#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <QVector3D>

#include "../render/mesh.h"
#include "../core/gameComponent.h"
#include "transform.h"

class GameObject{

private:
    std::string name;
    Transform transform;
    GameObject * parent;

    Mesh mesh;
    bool containsMesh = false;

    std::vector<GameObject *> children;
    std::vector<GameComponent * > gameComponents;

public:
    GameObject();
    GameObject( std::string name, GameObject * parent);
    GameObject( std::string name, std::string filename , QVector3D meshColor, GameObject * parent );

    void setFaces( const std::vector< std::vector< int > > &faces );
    const std::vector< std::vector< int > > getFaces();

    void addChild( GameObject * newChildren );
    const std::vector<GameObject *> getChildren();

    void setTransform(const Transform &newTransform);
    Transform &getTransform();

    void setMesh( Mesh &mesh );
    Mesh &getMesh();

    void addComponent( GameComponent * component );
    void removeComponent( GameComponent * component);

    bool hasMesh();

    void input();
    void update();
    void render();

    void move( QVector3D translation );
    void rotate( QVector3D axis, float angle );
    void scale( QVector3D scale );

    void applyTransformation( Transform &transform );
    void drawMesh( QOpenGLShaderProgram * program  );

    QMatrix4x4 getModel();

    const std::string &getName() const;
    void setName(const std::string &newName);

    void resetModelMatrix();

};


#endif // GAMEOBJECT_H
