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

    bool isMovable;
    bool isCollectible = false;
    bool isDestroyed = false;

    std::vector<GameObject *> children;
    QVector3D worldPosition;

    QVector3D predScale; // for setLastParent
    bool isTelekinesis = false ;

public:

    GameObject();
    GameObject( std::string name, GameObject * parent = nullptr );

    void addChild( GameObject * newChildren );
    void removeChild( GameObject *newChildren );

    void move( QVector3D translation );
    void move( float x, float y , float z );
    void rotate( QVector3D axis, float angle );
    void rotate( QQuaternion rotate );
    void scale( QVector3D scale );
    void scale( float scale );
    void setHeight(float height);

    const std::string &getName() const;
    void setName(const std::string &newName);

    void setTransform( Transform * newTransform);
    Transform *getTransform();

    void destroy();

    const QMatrix4x4 getModel();

    QVector3D& getWorldPosition();
    const std::vector<GameObject *> getChildren();

    GameObject * getParent();
    void setParent(GameObject *newParent);
    void setLastParent();
    bool getIsTelekinesis() const;
    void setIsTelekinesis(bool newIsTelekinesis);
    bool getIsMovable() const;
    void setIsMovable(bool newIsMovable);
    bool getIsCollectible() const;
    void setIsCollectible(bool newIsCollectible);
    bool getIsDestroyed() const;
    void setIsDestroyed(bool newIsDestroyed);
};




#endif // GAMEOBJECT_H
