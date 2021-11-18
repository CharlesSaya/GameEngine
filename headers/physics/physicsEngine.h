#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H


class physicsEngine
{
public:
    physicsEngine();
    void computeForces();
private:
    float deltaTime;
    float gravity = -9.81;

};

#endif // PHYSICSENGINE_H
