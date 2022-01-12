#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H


#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>

#include "headers/random.h"

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

    float spawnRange;
    bool renderPoint = false;

    int spriteNumber;

    QVector3D particlesSpawnPoint;
    QVector3D particleDirection;
    QVector3D color;

    QOpenGLContext * context;
    QOpenGLFunctions_3_3_Core * glFuncs;

    float maxParticles, newParticles;
    uint lastUsedParticle, particlesCount;

    std::vector<Texture> sprites;

    QOpenGLBuffer vBuffer, positionsBuffer, colorsBuffer;

    GameObjectCamera * camera;

    std::vector<Particle> particles;
    std::vector<QVector4D> positions;
    std::vector<QVector4D> colors;

    std::vector<VertexData> particleGeometry;

public:

    ParticleGenerator();
    ParticleGenerator( float number, std::vector<Texture> &sprites, QVector3D particlesSpawnPoint, QVector3D particleDirection, bool renderPoints  );
    ParticleGenerator( float number, QVector3D particlesSpawnPoint, QVector3D particleDirection, QVector3D color, bool renderPoints  );
    void initBuffers();
    void initGeometry();

    uint firstUnusedParticle();
    void respawnParticle();
    void sortParticles();
    void update(float dt );
    void render( Shader * shader );

    void setCamera(GameObjectCamera *newCamera);
    float getRange() const;
    void setRange(float newRange);
};

#endif // PARTICLEGENERATOR_H
