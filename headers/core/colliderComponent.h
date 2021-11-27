#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <QOpenGLFunctions_3_1>

#include "headers/render/mesh.h"
#include "headers/core/gameComponent.h"

class ColliderComponent : public GameComponent{


private:
    Mesh mesh;
    boolean moveable;

public:
    ColliderComponent( Mesh &mesh );
    void input(QKeyEvent *key);
    void update(float step);
    void render(const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition);
    boolean getMoveable() const;
    void setMoveable(boolean newMoveable);
    Mesh &getMesh();
    void setMesh(const Mesh &newMesh);
};

#endif // COLLIDERCOMPONENT_H
