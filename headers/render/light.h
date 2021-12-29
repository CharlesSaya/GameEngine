#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

#include "headers/render/shader.h"

class Light{
public:
    Light();
    Light(  QVector3D lightPosition );
    void loadLight( Shader * shader );

    const QVector3D &getLightPosition() const;

private:
    QVector3D lightPosition;
    Shader * shader;
};

#endif // LIGHT_H
