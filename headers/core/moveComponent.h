#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "transform.h"
#include "gameComponent.h"


class MoveComponent : public GameComponent{

signals:

private:
    float deltaTime;
    Transform transform;

public:

    MoveComponent( float deltaTime, Transform transform );
    void input() override;
    void update() override;
    void render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) override;
};

#endif // MOVECOMPONENT_H
