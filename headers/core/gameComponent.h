#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <QKeyEvent>

class GameComponent : public QObject
{

    Q_OBJECT

protected:
    template<typename Base, typename T> bool instanceof( const T * ptr );

public:

    virtual void input( QKeyEvent * key ) = 0 ;
    virtual void update( float step ) = 0 ;
    virtual void render( const QMatrix4x4& model,const  QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) = 0 ;

private:

};

template<typename Base, typename T> inline bool instanceof( T *ptr) {
    return dynamic_cast<Base*>(ptr) != nullptr;
}

#endif // GAMECOMPONENT_H
