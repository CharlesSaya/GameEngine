#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>


class Light{

public:

    const QVector3D &getLightPosition() const;

    const QVector3D &getLightColor() const;

protected:
    QVector3D lightPosition;
    QVector3D lightColor;
};

#endif // LIGHT_H
