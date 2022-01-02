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

void CoreEngine::mousePressEvent( QMouseEvent *event){

    this->game->pressedMouse( event );

}

void CoreEngine::mouseReleaseEvent( QMouseEvent *event){
    this->game->releasedMouse( event );

}

void CoreEngine::wheelEvent(QWheelEvent *event){
    this->game->mouseWheel( event );
}

void CoreEngine::mouseMoveEvent(QMouseEvent *e){
    if( e->localPos().x() > this->width() -10. || e->localPos().x() < 10. ){
        this->cursor().setPos( mapToGlobal( QPoint( width()/2, e->localPos().y() ) ) );
        this->game->mouseMoved( QVector2D( this->cursor().pos() ), true);
    }
    else
        this->game->mouseMoved( QVector2D( e->localPos() ), false);

//    float hAngle = 0., vAngle = 0.;
//    float sensitivity = 0.1f;
//    vAngle = mousePressPosition.y() -  e->localPos().y() ;
//    vAngle *= sensitivity * 3.;

//    if( e->localPos().x() > this->width() -10. || e->localPos().x() < 10. ){

//        mousePressPosition = QVector2D( width()/2, e->localPos().y() );
//        this->cursor().setPos( mapToGlobal( QPoint( mousePressPosition.x(), mousePressPosition.y() ) ) );
//        return;

//    }
//    else
//    {
//        hAngle = e->localPos().x() -mousePressPosition.x() ;
//        hAngle *= sensitivity;
//        mousePressPosition = QVector2D( e->localPos() );
//    }

//    yaw += hAngle;
//    pitch += vAngle;

//    if( pitch > 89.0f) pitch = 89.0f;
//    if( pitch < -89.0f) pitch = -89.0f;

//    this->game->getCamera()->rotate( pitch, yaw );
}


void CoreEngine::keyPressEvent(QKeyEvent *key){
    float movementSpeed = deltaTime;
    this->game->keyPressed( key );

//    switch( key->key() ){

//        case Qt::Key_Z:
//            this->game->getCamera()->move( this->game->getCamera()->getCameraForward() * movementSpeed * 2. );
//            break;

//        case Qt::Key_S:
//            this->game->getCamera()->move( -this->game->getCamera()->getCameraForward() * movementSpeed * 2. );
//            break;

//        case Qt::Key_Q:
//            this->game->getCamera()->move( this->game->getCamera()->getRight() * movementSpeed * 2. );
//            break;

//        case Qt::Key_D:
//            this->game->getCamera()->move( this->game->getCamera()->getLeft() * movementSpeed * 2. );
//            break;

//    }

}

void CoreEngine::keyReleaseEvent(QKeyEvent *key){
    this->game->keyReleased( key );

}

void CoreEngine::timerEvent(QTimerEvent *)
{

    accumulator += deltaTime;
    int cpt = 0;
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

//       glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);


    //Start timer   -------------------------------------------------------------------------------
    time.start();
    timer.start( this->renderStep, this );
    initGame();

}

void CoreEngine::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    game->setProjection( aspect );
    renderingEngine.screenResized( this->width(), this->height() );
}

void CoreEngine::initGame(){

    // Physics Engine  -----------------------------------------------------------------------
    physicsEngine = PhysicsEngine( this->fixedStep );
    colliderEngine = ColliderEngine( this->fixedStep );
    renderingEngine = RenderingEngine( this->renderStep );

    // Game  --------------------------------------------------------------------------------

    this->game->setCamera( camera );
    this->game->setPhysicsEngine( physicsEngine );
    this->game->setColliderEngine( colliderEngine );
    this->game->setRenderingEngine( renderingEngine );
    this->game->initGame();

}

void CoreEngine::paintGL()
{

    float currentFrame = time.elapsed()/1000.;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->game->render( deltaTime );

}
