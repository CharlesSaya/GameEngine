#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>

#include "headers/render/shader.h"
#include "headers/render/texture.h"
#include "headers/render/vertexData.h"
#include "headers/core/gameObjectCamera.h"

struct Particle{
    QVector3D position, speed;
    QVector4D color;

    float size, angle, weight;
    float life;
    float cameradistance;

    Particle() : position(QVector3D()), speed(QVector3D()), color(QVector4D()), life(0.0f) { }

    bool operator<(const Particle& that) const {
        return this->cameradistance > that.cameradistance;
    }
};

class ParticleGenerator {

private:

    QOpenGLContext * context;
    QOpenGLFunctions_3_3_Core * glFuncs;

    float number, newParticles;
    uint lastUsedParticle, particlesCount;

    Texture sprite;

    QOpenGLBuffer vBuffer, positionsBuffer, colorsBuffer;

    GameObjectCamera * camera;

    std::vector<Particle> particles;
    std::vector<QVector4D> positions;
    std::vector<QVector4D> colors;
    std::vector<VertexData> particleData;

public:

    ParticleGenerator();
    ParticleGenerator( float number, Texture &sprite );

    void initBuffers();
    void initGeometry();

    uint firstUnusedParticle();
    void respawnParticle();
    void sortParticles();
    void update(float dt );
    void render( Shader * shader );

    void setCamera(GameObjectCamera *newCamera);
};

#endif // PARTICLEGENERATOR_H
