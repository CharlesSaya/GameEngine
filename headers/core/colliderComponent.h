#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <QOpenGLFunctions_3_1>

#include "headers/render/mesh.h"
#include "headers/core/gameComponent.h"

class ColliderComponent{


private:
    bool moveable;

public:
    ColliderComponent();

    void update(float step);
    void render(const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection, const QVector3D &cameraPosition);

    bool getMoveable() const;
    void setMoveable(bool newMoveable);
};

#endif // COLLIDERCOMPONENT_H
