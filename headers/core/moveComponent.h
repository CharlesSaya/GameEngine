#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "headers/render/terrain.h"

#include "transform.h"
#include "gameComponent.h"


class MoveComponent : public GameComponent{

protected:
    bool eventFilter( QObject * obj,  QEvent * event) override;

public:

    MoveComponent( float deltaTime, Transform &transform, Terrain & terrain );
    void input( QKeyEvent * key  ) override;

    void update() override;
    void render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) override;

private:
    float deltaTime;
    float movementSpeed = 0.1;
    Terrain terrain;
    Transform &transform;
};

#endif // MOVECOMPONENT_H
