#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include <QTimer>
#include <QObject>
#include <QVector3D>
#include <QKeyEvent>
#include <QScrollEvent>

#include "headers/core/gameObject.h"
#include "headers/core/gameObjectMesh.h"
#include "headers/core/soundEngine.h"

#include "headers/render/ray.h"
#include "headers/render/shader.h"

class PlayerComponent : public QObject {
    Q_OBJECT


public slots:

    void pressedInput( QMouseEvent * key );
    void releasedInput( QMouseEvent * key );
    void wheelScrolled( QWheelEvent * scroll );
    void deactivateWheel();
    void update( QVector3D &playerPosition, QVector3D playerDirection );
    void hasMovedChild( QSet<uint>  inputsMoves);
    void hasStoppedChild( QSet<uint>  inputsMoves);
public:

    PlayerComponent( Shader * shader, QObject * parent = 0 );
    PlayerComponent( QVector3D &playerPosition, QVector3D &playerDirection, Shader * shader, QObject * parent = 0  );

    Ray & castRay(QVector3D target);

    void telekinesis(  GameObject * player, GameObject * go );

    bool telekinesisActivated();

    void drawRay( const QMatrix4x4& view, const QMatrix4x4& projection );
    void setPositionChild( GameObject * go,GameObject * child  );
    const Ray &getRay() const;
    void setRay(const Ray &newRay);

    void moveChildZ();
    int getCollectibleNumber() const;
    void setCollectibleNumber(int newCollectibleNumber);
    void addCollectible();

private:

    Ray ray;
    QTimer * timer;
    Shader * shader;
    int wheelTimer = 200;

    int collectibleNumber = 0;

    QVector3D playerPosition;
    QVector3D playerDirection = QVector3D( 0.0, 0.0, -1.0);

    GameObject * linkedGO = nullptr ;

    QSet<uint> inputsMoves;
    QVector3D translationChildSpeed = QVector3D(0.0f,0.01f,0.0f);
    float speedZ = 0.01;

    QVector2D rangeY = QVector2D(-5.0f,5.0f);
    QVector2D rangeZ = QVector2D(3.0f,20.0f);


    bool leftMousePressed = false;
    bool rightMousePressed = false;
    bool useTelekinesis = false ;

    bool wheelUp = false;
    bool wheelDown = false;

};

#endif // PLAYERCOMPONENT_H
