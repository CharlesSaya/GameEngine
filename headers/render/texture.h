#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include "headers/render/shader.h"

class Texture{

public:
    Texture();
    Texture( std::string file, std::string location );

    void initTexture();
    void bindTexture( uint textureUnit, Shader * shader ) const;
    void unbindTexture( uint textureUnit ) const;

    QOpenGLTexture * getTexture();

    QImage &getImage();
    void setImage(const QImage &newImage);

    const std::string &getLocation() const;
    void setLocation(const std::string &newLocation);

private:
    std::string location;
    QImage image;
    QOpenGLTexture * texture;
    QOpenGLContext * context;
    QOpenGLFunctions_3_3_Core * glFuncs;
};

#endif // TEXTURE_H
