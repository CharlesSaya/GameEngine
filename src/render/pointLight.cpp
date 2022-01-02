#include "headers/render/pointLight.h"


PointLight::PointLight(){

}

PointLight::PointLight(  float constant, float linear, float quadratic, QVector3D lightPosition, QVector3D &lightColor ){
    this->lightPosition = lightPosition;
    this->lightColor = lightColor;

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

float PointLight::getConstant() const
{
    return constant;
}

float PointLight::getLinear() const
{
    return linear;
}

float PointLight::getQuadratic() const
{
    return quadratic;
}
