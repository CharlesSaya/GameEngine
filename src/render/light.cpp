#include "headers/render/light.h"

Light::Light(){

}

Light::Light(  QVector3D lightPosition){
    this->lightPosition = lightPosition;
}

void Light::loadLight( Shader * shader ){
    shader->useShaderProgram();
    shader->setUniformValue( "lightPosition", this->lightPosition );
}

const QVector3D &Light::getLightPosition() const
{
    return lightPosition;
}

