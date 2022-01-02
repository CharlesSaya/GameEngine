#include "headers/render/light.h"

const QVector3D &Light::getLightPosition() const
{
    return lightPosition;
}

const QVector3D &Light::getLightColor() const
{
    return lightColor;
}

