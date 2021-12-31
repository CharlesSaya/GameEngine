#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "headers/render/light.h"

class DirectionalLight : public Light{

private:

public:

    DirectionalLight();
    DirectionalLight( QVector3D lightPosition, QVector3D &ambient, QVector3D &diffuse, QVector3D &specular );

    QVector3D getDirection();

};


#endif // DIRECTIONALLIGHT_H
