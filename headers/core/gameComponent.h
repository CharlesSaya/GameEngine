#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <QKeyEvent>

class GameComponent : public QObject
{
public:

    virtual void input() = 0 ;
    virtual void update() = 0 ;
    virtual void render( const QMatrix4x4& model,const  QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) = 0 ;
private:



};

#endif // GAMECOMPONENT_H
