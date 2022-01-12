#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <QImage>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>

#include "headers/render/shader.h"
#include "headers/render/texture.h"
#include "headers/render/vertexData.h"

class RenderText{

public:
    RenderText();
    RenderText( Texture &fontAtlas  );

    void initBuffers(  );
    void initTexture(  );
    void renderText( std::string text, QVector2D position, QVector2D screenWidth, int size, Shader * shader );

private:

    uint textureID;

    QOpenGLContext * context;
    QOpenGLFunctions_3_3_Core * glFuncs;

    std::vector<VertexData> quad;

    QOpenGLBuffer vBuffer, tBuffer;

    Texture fontAtlas;

};

#endif // RENDERTEXT_H
