#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>


class Light{

public:

    const QVector3D &getLightPosition() const;

    const QVector3D &getAmbient() const;
    const QVector3D &getDiffuse() const;
    const QVector3D &getSpecular() const;

protected:
    QVector3D lightPosition;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
};

#endif // LIGHT_H
