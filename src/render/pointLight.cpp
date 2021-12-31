#include "headers/render/pointLight.h"


PointLight::PointLight(){

}

PointLight::PointLight(  float constant, float linear, float quadratic, QVector3D lightPosition, QVector3D &ambient, QVector3D &diffuse, QVector3D &specular ){
    this->lightPosition = lightPosition;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;

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
