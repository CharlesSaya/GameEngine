#include <math.h>
#include "headers/core/coreEngine.h"

#include <QMouseEvent>

CoreEngine::CoreEngine(int frames, Game game, QWidget *parent) :
    QOpenGLWidget(parent),
    game( game ),
    frames(frames)
{
    this->setWindowTitle( QString   ( QString::number( frames )) + "FPS" );
    this->setMouseTracking(true);
    mousePressPosition =QVector2D( width()/2, height()/2 );
}

CoreEngine::~CoreEngine(){
    makeCurrent();
    doneCurrent();
}

void CoreEngine::mouseMoveEvent(QMouseEvent *e){

    float sensitivity = 0.1f;
    QPoint center = ( mapToGlobal( QPoint( width()/2, height()/2) ) );

    float diffX = mousePressPosition.x() - float(center.x()) ;
    diffX *= sensitivity;

    float diffY = float(center.y()) - mousePressPosition.y() ;
    diffY *= sensitivity;

    float yaw = -90. + diffX;
    float pitch = diffY;

    if( pitch > 89.0f) pitch = 89.0f;
    if( pitch < -89.0f) pitch = -89.0f;

    this->game.getCamera().rotate( pitch, yaw );

    mousePressPosition = QVector2D( e->localPos() );

}

void CoreEngine::timerEvent(QTimerEvent *)
{
    update();

}

void CoreEngine::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    //Start timer   -------------------------------------------------------------------------------
    time.start();
    timer.start(1000/frames , this);
    game.initGame();

}


void CoreEngine::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 100.0, fov = 80.0;

    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void CoreEngine::keyPressEvent(QKeyEvent *key){
    float movementSpeed = 10. * deltaTime;
    Camera &camera = this->game.getCamera();
    switch( key->key() ){
        case Qt::Key_Z:
            camera.move( camera.cameraForward, movementSpeed );
            break;
        case Qt::Key_S:
            camera.move( -camera.cameraForward, movementSpeed );
            break;
        case Qt::Key_Q:
            camera.move( camera.getRight(), movementSpeed );
            break;
        case Qt::Key_D:
            camera.move( camera.getLeft(), movementSpeed );
            break;
    }
    update();
}

void CoreEngine::paintGL()
{
    float currentFrame = time.elapsed()/1000.;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    game.input( time.elapsed()/1000. );
    game.update( time.elapsed()/1000.  );

    game.render( projection );

}
