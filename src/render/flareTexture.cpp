#include "headers/render/flareTexture.h"

const Texture &FlareTexture::getTexture() const
{
    return texture;
}

float FlareTexture::getScale() const
{
    return scale;
}

const QVector2D &FlareTexture::getScreenPos() const
{
    return screenPos;
}

void FlareTexture::setTexture(const Texture &newTexture)
{
    texture = newTexture;
}

void FlareTexture::setScale(float newScale)
{
    scale = newScale;
}

void FlareTexture::setScreenPos(const QVector2D &newScreenPos)
{
    screenPos = newScreenPos;
}
