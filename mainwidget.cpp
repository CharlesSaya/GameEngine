#include <math.h>
#include "mainwidget.h"

#include <QMouseEvent>

MainWidget::MainWidget(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    rockTexture(0),
    angularSpeed(0),
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
    delete geometries;
    doneCurrent();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (orbitalMode)
        return;
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (orbitalMode)
        return;

    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}


void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.8;

    if( orbitalMode) {

        orbitalCamera.cameraPosition = QVector3D( 20.* sin( time.elapsed() / 1000. ), orbitalCamera.cameraPosition.y(),
                                                   -20. - 20. *( - cos(time.elapsed() /  1000. )));
    }
    else {
        if( rotationMode ){
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.,1.,0.), -rotationSpeed*( time.elapsed() / 1000. ));
        }
        else{
            if (angularSpeed < 0.01) {
                angularSpeed = 0.0;
            } else {
                rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
            }
        }
    }
    update();

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

    geometries = new GeometryEngine;

    // Cameras  -------------------------------------------------------------------------------
    QVector3D cameraPosition = QVector3D(.0,1.0,4.);
    QVector3D orbitalCameraPosition = QVector3D(.0,15.0,-0.);

    QVector3D cameraTarget = QVector3D(0.0,0.,0.0);

    camera = Camera( cameraPosition, cameraTarget );
    orbitalCamera = Camera( orbitalCameraPosition, cameraTarget, true );

    std::string sphereObj = "../GameEngine/sphere.obj";
    std::string bunnyObj = "../GameEngine/bunny.obj";

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

    //Start timer   -------------------------------------------------------------------------------
    time.start();
    timer.start(1000/frames , this);
}

void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
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
    rockTexture = new QOpenGLTexture(QImage(":/rock.png").mirrored());
    rockTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    rockTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    rockTexture->setWrapMode(QOpenGLTexture::Repeat);

    grassTexture = new QOpenGLTexture(QImage(":/grass.png").mirrored());
    grassTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    grassTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    grassTexture->setWrapMode(QOpenGLTexture::Repeat);

    snowTexture = new QOpenGLTexture(QImage(":/snowrocks.png").mirrored());
    snowTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    snowTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    snowTexture->setWrapMode(QOpenGLTexture::Repeat);

    heightMap = new QOpenGLTexture( QImage(":/heightmap-1024x1024.png") );
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
            if (rotationMode)
                rotationSpeed+=20.;
            else
                camera.cameraPosition += cameraSpeed * cameraFront;
            break;
        case Qt::Key_Down:
            if (rotationMode)
                if (rotationSpeed - 20. < 0.)
                    rotationSpeed = 0.;
                else
                    rotationSpeed-=20.;
            else
                camera.cameraPosition -= cameraSpeed * cameraFront;
            break;
        case Qt::Key_Right:
            if (rotationMode)
                return;
            else
                camera.cameraPosition += cameraSpeed * QVector3D::crossProduct(cameraFront,up).normalized() ;
            break;
        case Qt::Key_Left:
            if (rotationMode)
                return;
            else
                camera.cameraPosition -= cameraSpeed * QVector3D::crossProduct(cameraFront,up).normalized() ;
            break;
        case Qt::Key_C:
            time.restart();
            if( orbitalMode ){
                orbitalMode = false;
            }
            else{
                orbitalMode = true;
            }
            break;
        case Qt::Key_R:
            time.restart();
            if (!orbitalMode){
                if( rotationMode )
                    rotationMode = false;

                else{
                    rotationSpeed = 5.;
                    rotationMode = true;
                }
            }
            break;

    }
    update();
}

void MainWidget::paintGL()
{

    // Clear color and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    rockTexture->bind(0);
    grassTexture->bind(1);
    snowTexture->bind(2);
    heightMap->bind(3);

    if (orbitalMode)
        view = orbitalCamera.getViewMatrix();
    else
        view = camera.getViewMatrix() ;

    sun->resetTransformMatrix();
    sun->rotate( QVector3D(0.,1.,0.), 20 * -rotationSpeed * ( time.elapsed()/1000. ) );

    earthOrbit->resetTransformMatrix();
    earthOrbit->rotate( QVector3D( 0.,1.,0. ), -rotationSpeed * ( time.elapsed()/1000. ) );
    earthOrbit->translate( QVector3D( 3., 0.0, 0.0 ) );
    earthOrbit->rotate( QVector3D( 0.,0.,1. ), -23.44  );

    earth->resetTransformMatrix();
    earth->scale( QVector3D( .5, .5, .5 ) );
    earth->rotate( QVector3D(0.,1.,0.), -rotationSpeed * ( time.elapsed()/1000. ) );

    moonOrbit->resetTransformMatrix();
    moonOrbit->rotate( QVector3D(0.,1.,0.), 10 * -rotationSpeed * ( time.elapsed()/1000. ) );
    moonOrbit->translate(QVector3D( 1.0, 0.0, 0.0 ));

    moon->resetTransformMatrix();
    moon->scale( QVector3D( .15, .15, .15) );
    moon->rotate( QVector3D(0.,1.,0.), -10 * rotationSpeed * ( time.elapsed()/1000. ) );

    program.setUniformValue( "rockTexture", 0 );
    program.setUniformValue( "grassTexture", 1 );
    program.setUniformValue( "snowTexture", 2 );
    program.setUniformValue( "heightMap", 3 );

    sceneGraph.draw( sceneGraph.getRoot(), view, projection, &program );

    rockTexture->release();
    grassTexture->release();
    snowTexture->release();
    heightMap->release();

}
