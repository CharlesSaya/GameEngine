#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <QVector3D>
#include <QKeyEvent>

#include "headers/render/shader.h"

#include "headers/core/transform.h"


class GameObject : public QObject{
    Q_OBJECT

protected:

    std::string name;
    Transform * transform;
    GameObject * parent;
    GameObject * lastParent;

    std::vector<GameObject *> children;
    QVector3D worldPosition;
public:

    GameObject();
    GameObject( std::string name, GameObject * parent = nullptr );

    void addChild( GameObject * newChildren );
    void removeChild( GameObject *newChildren );

    void move( QVector3D translation );
    void move( float x, float y , float z );
    void rotate( QVector3D axis, float angle );
    void rotate( QQuaternion rotate );
    void addRotate( QQuaternion rotate );
    void scale( QVector3D scale );
    void scale( float scale );
    void setHeight(float height);

    const std::string &getName() const;
    void setName(const std::string &newName);

    void setTransform( Transform * newTransform);
    Transform *getTransform();

    const QMatrix4x4 getModel();

    QVector3D& getWorldPosition();
    const std::vector<GameObject *> getChildren();

    GameObject * getParent();
    void setParent(GameObject *newParent);
    void setLastParent();
    void attract(float speed);
    void push(float speed);
};




#endif // GAMEOBJECT_H
