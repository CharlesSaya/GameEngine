#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLTexture>

#include "headers/render/shader.h"

class Texture{

public:
    Texture();
    Texture( std::string file, std::string location );
    void initTexture();
    void bindTexture( uint textureUnit, Shader * shader ) const;
    void unbindTexture() const;

    QOpenGLTexture * getTexture();

    QImage &getImage();
    void setImage(const QImage &newImage);

    const std::string &getLocation() const;
    void setLocation(const std::string &newLocation);

private:
    std::string location;
    QImage image;
    QOpenGLTexture * texture;

};

#endif // TEXTURE_H
