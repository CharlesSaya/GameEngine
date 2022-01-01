#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "headers/render/light.h"

class PointLight : public Light{
private:
    float constant;
    float linear;
    float quadratic;

public:

    PointLight();
    PointLight( float constant, float linear, float quadratic, QVector3D lightPosition, QVector3D &ambient, QVector3D &diffuse, QVector3D &specular );

    float getConstant() const;
    float getLinear() const;
    float getQuadratic() const;
};

#endif // POINTLIGHT_H
