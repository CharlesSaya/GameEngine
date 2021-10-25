#include <math.h>
#include "headers/mainwidget.h"

#include <QMouseEvent>

MainWidget::MainWidget(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    rockTexture(0),
    frames(frames)
{
    this->setWindowTitle( QString( QString::number( frames )) + "FPS" );
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete rockTexture;
    doneCurrent();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{

    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{

    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

}


void MainWidget::timerEvent(QTimerEvent *)
{

    update();

}

void MainWidget::initScene(){
    // Cameras  -------------------------------------------------------------------------------
    QVector3D cameraPosition = QVector3D(.0,1.0,4.);

    QVector3D cameraTarget = QVector3D(0.0,0.,0.0);

    camera = Camera( cameraPosition, cameraTarget );

    std::string sphereObj = "../GameEngine/objects/sphere.obj";
    std::string bunnyObj = "../GameEngine/objects/bunny.obj";

    //Build scene graph  -------------------------------------------------------------------------------

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
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void MainWidget::initTextures()
{
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
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = .1, zFar = 100.0, fov = 80.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::keyPressEvent(QKeyEvent *key){
    float cameraSpeed =.2;

    switch( key->key() ){
        case Qt::Key_Up:
            camera.cameraPosition -= cameraSpeed * camera.cameraDirection;
            break;
        case Qt::Key_Down:
            camera.cameraPosition += cameraSpeed * camera.cameraDirection;
            break;
        case Qt::Key_Right:
            camera.cameraPosition += cameraSpeed * camera.cameraRight ;
            break;
        case Qt::Key_Left:
            camera.cameraPosition -= cameraSpeed * camera.cameraRight ;
            break;  
    }
    update();
}

void MainWidget::paintGL()
{

    // Clear color and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    view = camera.getViewMatrix() ;

    sun->resetTransformMatrix();
    sun->rotate( QVector3D(0.,1.,0.), - 50 * ( time.elapsed()/1000. ) );

    earthOrbit->resetTransformMatrix();
    earthOrbit->rotate( QVector3D( 0.,1.,0. ), -20 * ( time.elapsed()/1000. ) );
    earthOrbit->translate( QVector3D( 3., 0.0, 0.0 ) );
    earthOrbit->rotate( QVector3D( 0.,0.,1. ), -23.44  );

    earth->resetTransformMatrix();
    earth->scale( QVector3D( .5, .5, .5 ) );
    earth->rotate( QVector3D(0.,1.,0.), -20 * ( time.elapsed()/1000. ) );

    moonOrbit->resetTransformMatrix();
    moonOrbit->rotate( QVector3D(0.,1.,0.), 10 * -20 * ( time.elapsed()/1000. ) );
    moonOrbit->translate(QVector3D( 1.0, 0.0, 0.0 ));

    moon->resetTransformMatrix();
    moon->scale( QVector3D( .15, .15, .15) );
    moon->rotate( QVector3D(0.,1.,0.), -10 * 20 * ( time.elapsed()/1000. ) );

    sceneGraph.draw( sceneGraph.getRoot(), view, projection, &program );

}
