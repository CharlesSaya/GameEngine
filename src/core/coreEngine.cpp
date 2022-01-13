#include "headers/core/coreEngine.h"


CoreEngine::CoreEngine(){

}

/**
 * @brief Constructeur
 * @param frames
 * @param parent
 */

CoreEngine::CoreEngine(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    frames(frames)
{
    QCursor cursor(Qt::BlankCursor);
    QApplication::setOverrideCursor(cursor);
    QApplication::changeOverrideCursor(cursor);
//    this->resize( 1366, 768 );

    this->renderStep = frames / 1000.f;
    this->grabMouse();
    this->grabKeyboard();
    this->game = new Game(this);

    this->setMouseTracking(true);
    this->setWindowTitle( QString   ( QString::number( frames ) ) + "FPS" );

    mousePressPosition =QVector2D( width()/2, height()/2 );
}

/**
 * @brief Slot activé par l'appui d'une touche de souris
 * @param event
 */

void CoreEngine::mousePressEvent( QMouseEvent *event){

    this->game->pressedMouse( event );

}

/**
 * @brief Slot activé par le relachement d'une touche de souris
 * @param event
 */

void CoreEngine::mouseReleaseEvent( QMouseEvent *event){
    this->game->releasedMouse( event );

}

/**
 * @brief Slot récupérant les évenements de la molette
 * @param event
 */

void CoreEngine::wheelEvent(QWheelEvent *event){
    this->game->mouseWheel( event );
}

/**
 * @brief Slot activé par le mouvement de la souris souris
 * @param e
 */

void CoreEngine::mouseMoveEvent(QMouseEvent *e){
    if( e->localPos().x() > this->width() -10. || e->localPos().x() < 10. ){
        this->cursor().setPos( mapToGlobal( QPoint( this->width()/2, e->localPos().y() ) ) );
        this->game->mouseMoved( QVector2D( this->cursor().pos() ), true, false );
    }else if ( e->localPos().y() > this->height() -10. || e->localPos().y() < 10. ){
        this->cursor().setPos( mapToGlobal( QPoint( e->localPos().x(), this->height()/2 ) ) );
        this->game->mouseMoved( QVector2D( this->cursor().pos() ), false, true );

    }
    else
        this->game->mouseMoved( QVector2D( e->localPos() ), false, false );

}

/**
 * @brief Slot activé par l'appui d'une touche de souris
 * @param key
 */

void CoreEngine::keyPressEvent(QKeyEvent *key){
    float movementSpeed = deltaTime;
    this->game->keyPressed( key );

}

/**
 * @brief Slot activé par le relachement d'une touche de souris
 * @param key
 */

void CoreEngine::keyReleaseEvent(QKeyEvent *key){
    this->game->keyReleased( key );

}

/**
 * Event activé à chaque boucle de rendu
 */

void CoreEngine::timerEvent(QTimerEvent *)
{

    accumulator += deltaTime;
    int cpt = 0;
    while( accumulator > this->fixedStep ){
        game->update( this->fixedStep );
        accumulator -= this->fixedStep;
        cpt++;
    }
    update();
}

/**
 * @brief Initialise les fonctions et le contexte d'OpenGL
 */

void CoreEngine::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

//       glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    time.start();
    timer.start( this->renderStep, this );
    initGame();

}

/**
 * @brief Slots appelé lorsque la fenêtre se redimensionne
 * @param w
 * @param h
 */

void CoreEngine::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    game->setProjection( aspect );
    renderingEngine.screenResized( this->width(), this->height() );
}

/**
 * @brief Initialisation du jeu
 */

void CoreEngine::initGame(){

    physicsEngine = PhysicsEngine( this->fixedStep );
    colliderEngine = ColliderEngine( this->fixedStep );
    renderingEngine = RenderingEngine( this->renderStep );

    this->game->setCamera( camera );
    this->game->setPhysicsEngine( physicsEngine );
    this->game->setColliderEngine( colliderEngine );
    this->game->setRenderingEngine( renderingEngine );
    this->game->initGame();

}

/**
 * @brief Fonction permettant l'affichage de la scène sur l'écran
 */

void CoreEngine::paintGL()
{

    float currentFrame = time.elapsed()/1000.;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->game->render( deltaTime );

}
