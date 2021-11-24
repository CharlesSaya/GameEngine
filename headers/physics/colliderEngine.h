#ifndef COLLIDERENGINE_H
#define COLLIDERENGINE_H

#include "headers/game/sceneGraph.h"
#include "headers/core/gameComponent.h"
#include "headers/core/colliderComponent.h"

class ColliderEngine{

public:
    ColliderEngine( SceneGraph &sceneGraph );
    void computeCollision();

private:
    SceneGraph & sceneGraph;
};

#endif // COLLIDERENGINE_H
