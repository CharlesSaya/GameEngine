#ifndef FLARETEXTURE_H
#define FLARETEXTURE_H

#include "headers/render/texture.h"

class FlareTexture{

public:

    FlareTexture(const Texture &texture, float scale);

    const Texture &getTexture() const;
    float getScale() const;
    const QVector2D &getScreenPos() const;

    void setTexture(const Texture &newTexture);
    void setScale(float newScale);
    void setScreenPos(const QVector2D &newScreenPos);

private:

    Texture texture;
    float scale;
    QVector2D screenPos;

};

#endif // FLARETEXTURE_H


inline FlareTexture::FlareTexture(const Texture &texture, float scale ) : texture(texture),
    scale(scale)
{}

