#include "headers/render/directionalLight.h"

DirectionalLight::DirectionalLight(){

}

DirectionalLight::DirectionalLight( QVector3D lightPosition, QVector3D &ambient, QVector3D &diffuse, QVector3D &specular ){
    this->lightPosition = lightPosition;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

QVector3D DirectionalLight::getDirection(){
    return QVector3D(0.0, 0.0, 0.0) - this->lightPosition;
}
