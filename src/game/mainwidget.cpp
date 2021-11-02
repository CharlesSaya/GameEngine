#include <math.h>
#include "headers/game/mainwidget.h"

#include <QMouseEvent>

MainWidget::MainWidget(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    rockTexture(0),
    frames(frames)
{
    this->setWindowTitle( QString( QString::number( frames )) + "FPS" );
    this->setMouseTracking(true);
    mousePressPosition =QVector2D( width()/2, height()/2 );
}

MainWidget::~MainWidget()
{
    makeCurrent();
    delete rockTexture;
    doneCurrent();
}

void MainWidget::mousePressEvent(QMouseEvent *e){

}

void MainWidget::mouseReleaseEvent(QMouseEvent *e){

}

void MainWidget::mouseMoveEvent(QMouseEvent *e){

    float sensitivity = 0.1f;
    QPoint center = ( mapToGlobal( QPoint( width()/2, height()/2) ) );

    float diffX = mousePressPosition.x() - float(center.x()) ;
    diffX *= sensitivity;

    float diffY = float(center.y()) - mousePressPosition.y() ;
    diffY *= sensitivity;

    yaw = -90. + diffX;
    pitch = diffY;

    if( pitch > 89.0f) pitch = 89.0f;
    if( pitch < -89.0f) pitch = -89.0f;

    this->camera.rotate( pitch, yaw );

    mousePressPosition = QVector2D( e->localPos() );

}

void MainWidget::timerEvent(QTimerEvent *)
{

    update();

}

void MainWidget::initScene(){

    // Cameras  -------------------------------------------------------------------------------
    QVector3D cameraPosition = QVector3D(.0,0.0,4.);
    QVector3D cameraTarget = QVector3D(.0,.0,0.);

    camera = Camera( cameraPosition, cameraTarget );

    //Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere.obj";
    std::string bunnyObj  = "../GameEngine/objects/bunny.obj";

    solarSystem = new GameObject( "Solar System", nullptr ) ;
    sun   = new GameObject( "Sun", sphereObj, QVector3D( 1., 1., 0. ), solarSystem);

    earthOrbit  = new GameObject( "Earth Orbit", solarSystem ) ;
    earth = new GameObject( "Earth", sphereObj, QVector3D( 0., 0., 1. ), earthOrbit );

    moonOrbit   = new GameObject( " Moon Orbit ", earthOrbit ) ;
    moon  = new GameObject( "Moon", sphereObj, QVector3D( 0.75, 0.75, 0.75 ), moonOrbit );

    solarSystem->addChild( sun );
    solarSystem->addChild( earthOrbit );

    earthOrbit->addChild( earth );
    earthOrbit->addChild( moonOrbit );

    moonOrbit->addChild( moon );

    sceneGraph = SceneGraph( solarSystem );
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    //Start timer   -------------------------------------------------------------------------------
    time.start();
    timer.start(1000/frames , this);

    this->initScene();
}

void MainWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    if (!program.link())
        close();

    if (!program.bind())
        close();
}

void MainWidget::initTextures()
{
    std::string textures[4] = { ":/textures/rock.png",
                               ":/textures/grass.png" ,
                               ":/textures/snowrocks.png",
                               ":/textures/heightmap-1024x1024.png" };

    rockTexture = new QOpenGLTexture(QImage(":/textures/rock.png").mirrored());
    rockTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    rockTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    rockTexture->setWrapMode(QOpenGLTexture::Repeat);

    grassTexture = new QOpenGLTexture(QImage(":/textures/grass.png").mirrored());
    grassTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    grassTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    grassTexture->setWrapMode(QOpenGLTexture::Repeat);

    snowTexture = new QOpenGLTexture(QImage(":/textures/snowrocks.png").mirrored());
    snowTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    snowTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    snowTexture->setWrapMode(QOpenGLTexture::Repeat);

    heightMap = new QOpenGLTexture( QImage(":/textures/heightmap-1024x1024.png") );
    heightMap->setMinificationFilter(QOpenGLTexture::Nearest);
    heightMap->setMagnificationFilter(QOpenGLTexture::Linear);
    heightMap->setWrapMode(QOpenGLTexture::Repeat);
}

void MainWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 100.0, fov = 80.0;

    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::keyPressEvent(QKeyEvent *key){
    float movementSpeed = 10. * deltaTime;

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

void MainWidget::paintGL()
{
    float currentFrame = time.elapsed()/1000.;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    view = camera.getViewMatrix() ;

    sun->resetModelMatrix();
    sun->rotate( QVector3D(0.,1.,0.), - 50. * ( time.elapsed()/1000. ) );

    earthOrbit->resetModelMatrix();
    earthOrbit->rotate( QVector3D( 0.,1.,0. ), -20 * ( time.elapsed()/1000. ) );
    earthOrbit->translate( QVector3D( 3., 0.0, 0.0 ) );
    earthOrbit->rotate( QVector3D( 0.,0.,1. ), -23.44  );

    earth->resetModelMatrix();
    earth->scale( QVector3D( .5, .5, .5 ) );
    earth->rotate( QVector3D(0.,1.,0.), -20 * ( time.elapsed()/1000. ) );

    moonOrbit->resetModelMatrix();
    moonOrbit->rotate( QVector3D(0.,1.,0.), 10 * -20 * ( time.elapsed()/1000. ) );
    moonOrbit->translate(QVector3D( 1.0, 0.0, 0.0 ));

    moon->resetModelMatrix();
    moon->scale( QVector3D( .15, .15, .15) );
    moon->rotate( QVector3D(0.,1.,0.), -10 * 20 * ( time.elapsed()/1000. ) );

    sceneGraph.draw( sceneGraph.getRoot(), view, projection, &program );

}
