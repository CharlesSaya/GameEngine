#ifndef PLAYER_H
#define PLAYER_H

#include "gameObject.h"
#include "headers/render/mesh.h"

#include "headers/render/terrain.h"

class Player{

public:
    Player();
    Player( GameObject & gameObject );
    void move( QVector3D translation, Terrain & terrain );
    void setHeight( float height);
    void setMesh(const Mesh &newMesh);

private:
    GameObject * player;
    Mesh mesh;
};

#endif // PLAYER_H
