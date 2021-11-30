#include "headers/core/coreEngine.h"


CoreEngine::CoreEngine(){

}

CoreEngine::CoreEngine(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    frames(frames)
{
    this->resize( 1366, 768 );

    this->renderStep = frames / 1000.f;
    this->grabMouse();
    this->grabKeyboard();
    this->game = new Game(this);

    this->setMouseTracking(true);
    this->setWindowTitle( QString   ( QString::number( frames ) ) + "FPS" );

    mousePressPosition =QVector2D( width()/2, height()/2 );

}

void CoreEngine::mouseMoveEvent(QMouseEvent *e){
    float hAngle = 0., vAngle = 0.;
    float sensitivity = 0.1f;
    vAngle = mousePressPosition.y() -  e->localPos().y() ;
    vAngle *= sensitivity * 3.;

    if( e->localPos().x() > this->width() -10. || e->localPos().x() < 10. ){

        mousePressPosition = QVector2D( width()/2, e->localPos().y() );
        this->cursor().setPos( mapToGlobal( QPoint( mousePressPosition.x(), mousePressPosition.y() ) ) );
        return;

    }
    else{

        hAngle = e->localPos().x() -mousePressPosition.x() ;
        hAngle *= sensitivity;

        mousePressPosition = QVector2D( e->localPos() );
    }

    yaw += hAngle;
    pitch += vAngle;

    if( pitch > 89.0f) pitch = 89.0f;
    if( pitch < -89.0f) pitch = -89.0f;

    this->camera->rotate( pitch, yaw );

}


void CoreEngine::keyPressEvent(QKeyEvent *key){
    float movementSpeed = deltaTime;
    this->game->keyPressed( key );

    switch( key->key() ){

        case Qt::Key_Z:
            camera->move( camera->getCameraForward() * movementSpeed * 2. );
            break;

        case Qt::Key_S:
            camera->move( -camera->getCameraForward() * movementSpeed * 2. );
            break;

        case Qt::Key_Q:
            camera->move( camera->getRight() * movementSpeed * 2. );
            break;

        case Qt::Key_D:
            camera->move( camera->getLeft() * movementSpeed * 2. );
            break;

    }

}

void CoreEngine::keyReleaseEvent(QKeyEvent *key){
    this->game->keyReleased( key );

    switch( key->key() ){



    }
}

void CoreEngine::timerEvent(QTimerEvent *)
{

    accumulator += deltaTime;

    while( accumulator > this->fixedStep ){
        game->update( this->fixedStep );
        accumulator -= this->fixedStep;
    }

    update();

}

void CoreEngine::initializeGL(){
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);

    //Start timer   -------------------------------------------------------------------------------
    time.start();
    timer.start( this->renderStep, this );
    initGame();

}

void CoreEngine::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    game->setProjection( aspect );

}

void CoreEngine::initGame(){

    // Physics Engine  -----------------------------------------------------------------------
    PhysicsEngine physicsEngine = PhysicsEngine( this->fixedStep );

    // Camera  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D( 2.0, 2.0, 1. );
    QVector3D cameraTarget   = QVector3D( .0, .0, .0 );
    const qreal zNear = .01, zFar = 100.0, fov = 80.0;

    camera = new Camera( cameraPosition, cameraTarget, fov, zNear, zFar );

    // Game  --------------------------------------------------------------------------------

    this->game->setCamera( camera );
    this->game->setPhysicsEngine( physicsEngine );
    this->game->initGame();

}

void CoreEngine::paintGL()
{

    float currentFrame = time.elapsed()/1000.;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->game->render();

}
