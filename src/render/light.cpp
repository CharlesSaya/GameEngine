#include "headers/render/light.h"

const QVector3D &Light::getLightPosition() const
{
    return lightPosition;
}

const QVector3D &Light::getAmbient() const
{
    return ambient;
}

const QVector3D &Light::getDiffuse() const
{
    return diffuse;
}

const QVector3D &Light::getSpecular() const
{
    return specular;
}

