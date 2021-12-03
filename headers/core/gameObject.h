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

    std::vector<GameObject *> children;

public:

    GameObject();
    GameObject( std::string name, GameObject * parent = nullptr );

    void addChild( GameObject * newChildren );

    void move( QVector3D translation );
    void rotate( QVector3D axis, float angle );
    void scale( QVector3D scale );

    void resetModelMatrix();

    const std::string &getName() const;
    void setName(const std::string &newName);

    void setTransform( Transform * newTransform);
    Transform *getTransform();

    const QMatrix4x4& getModel();
    QVector3D getWorldPosition();
    const std::vector<GameObject *> getChildren();

};




#endif // GAMEOBJECT_H
