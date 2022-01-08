#include "headers/render/directionalLight.h"

DirectionalLight::DirectionalLight(){

}

DirectionalLight::DirectionalLight( QVector3D lightPosition, QVector3D &lightColor ){
    this->lightPosition = lightPosition;
    this->lightColor = lightColor;
}

QVector3D DirectionalLight::getDirection(){
    return QVector3D(64.0, 0.0, 64.0) - this->lightPosition;
}
