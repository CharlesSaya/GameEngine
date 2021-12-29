#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include <QTimer>
#include <QObject>
#include <QVector3D>
#include <QKeyEvent>
#include <QScrollEvent>

#include "headers/core/gameObject.h"

#include "headers/render/ray.h"
#include "headers/render/shader.h"
#include "headers/render/shader.h"

class PlayerComponent : public QObject {
    Q_OBJECT


public slots:

    void pressedInput( QMouseEvent * key );
    void releasedInput( QMouseEvent * key );
    void wheelScrolled( QWheelEvent * scroll );
    void deactivateWheel();
    void update( QVector3D &playerPosition, QVector3D playerDirection );

public:

    PlayerComponent( Shader * shader, QObject * parent = 0 );
    PlayerComponent( QVector3D &playerPosition, QVector3D &playerDirection, Shader * shader, QObject * parent = 0  );

    Ray & castRay();

    void telekinesis(  GameObject * player, GameObject * go );

    bool telekinesisActivated();

    void drawRay( const QMatrix4x4& view, const QMatrix4x4& projection );

    const Ray &getRay() const;
    void setRay(const Ray &newRay);

private:

    Ray ray;
    QTimer * timer;
    Shader * shader;
    int wheelTimer = 300;

    QVector3D playerPosition;
    QVector3D playerDirection = QVector3D( 0.0, 0.0, -1.0);

    GameObject * linkedGO = nullptr ;

    bool leftMousePressed = false;
    bool rightMousePressed = false;

    bool wheelUp = false;
    bool wheelDown = false;

};

#endif // PLAYERCOMPONENT_H
