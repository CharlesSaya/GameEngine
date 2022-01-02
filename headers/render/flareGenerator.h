#ifndef FLAREGENERATOR_H
#define FLAREGENERATOR_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>

#include "headers/core/gameObjectCamera.h"

#include "headers/render/shader.h"
#include "headers/render/vertexData.h"
#include "headers/render/flareTexture.h"
#include "headers/render/directionalLight.h"

class FlareGenerator{

private:

    DirectionalLight sun;
    std::vector<FlareTexture> flareTextures;
    GameObjectCamera * camera;

    QOpenGLContext * context;
    QOpenGLFunctions_3_3_Core * glFuncs;

    QOpenGLBuffer vBuffer;
    std::vector<VertexData> particleData;

    QOpenGLBuffer positionsBuffer;
    std::vector<QVector4D> positions;

    int width = 1920, height = 1080;

public:

    FlareGenerator();
    FlareGenerator(const DirectionalLight &sun, const std::vector<FlareTexture> &textures);

    void initBuffers();
    void initGeometry();
    QVector2D sunScreenPosition();
    void computeFlaresPosition( QVector2D sunToCenter, QVector2D sunPosition );

    void screenResized( int width, int height  );

    void renderFlare( FlareTexture flare, Shader * shader );
    void render( float brightness, Shader * shader );
    void setCamera(GameObjectCamera *newCamera);
};

#endif // FLAREGENERATOR_H
