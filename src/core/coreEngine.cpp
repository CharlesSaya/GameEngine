#include "headers/core/coreEngine.h"


CoreEngine::CoreEngine(){

}

CoreEngine::CoreEngine(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    frames(frames)
{

//    QCursor cursor(Qt::BlankCursor);
//    QApplication::setOverrideCursor(cursor);
//    QApplication::changeOverrideCursor(cursor);

    this->grabMouse();
    this->grabKeyboard();

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

    this->camera.rotate( pitch, yaw );

}


void CoreEngine::keyPressEvent(QKeyEvent *key){
    float movementSpeed = deltaTime;

    switch( key->key() ){

        case Qt::Key_Z:
            camera.move( camera.cameraForward * movementSpeed * 2. );
            break;

        case Qt::Key_S:
            camera.move( -camera.cameraForward * movementSpeed * 2. );
            break;

        case Qt::Key_Q:
            camera.move( camera.getRight() * movementSpeed * 2. );
            break;

        case Qt::Key_D:
            camera.move( camera.getLeft() * movementSpeed * 2. );
            break;

        case Qt::Key_Up:

            player.move( QVector3D( 0.,0.,-1. ) * movementSpeed, terrain );
            update();
            break;

        case Qt::Key_Down:

            player.move( QVector3D( 0.,0.,1. ) * movementSpeed, terrain );
            update();
            break;

        case Qt::Key_Left:

            player.move( QVector3D( -1.,0.,0. ) * movementSpeed, terrain );
            update();
            break;

        case Qt::Key_Right:

            player.move( QVector3D( 1.,0.,0. ) * movementSpeed, terrain );
            update();
            break;
    }

}

void CoreEngine::timerEvent(QTimerEvent *)
{
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
    timer.start(1000/frames , this);

    initGame();
//    game.initGame();

}

void CoreEngine::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 100.0, fov = 80.0;

    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}


void CoreEngine::initGame(){

    // Shaders & Lights  -------------------------------------------------------------------------------

    shader = new Shader( "../GameEngine/shaders/base_vshader.glsl", "../GameEngine/shaders/base_fshader.glsl" );
    terrainShader = new Shader(  "../GameEngine/shaders/terrain_vshader.vert", "../GameEngine/shaders/terrain_fshader.frag" );

    Light light( QVector3D( 5.0, 4.0, -5.0) );
    light.loadLight( shader );
    light.loadLight( terrainShader );

    // Cameras  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D(2.0,2.0,1.);
    QVector3D cameraTarget   = QVector3D(.0,.0,.0);

    camera = Camera( cameraPosition, cameraTarget );

    // Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere/";
    std::string bunnyObj  = "../GameEngine/objects/bunny/";

    Texture heightMap = Texture( "../GameEngine/textures/Heightmap_Rocky.png", "heightMap" );
    Texture snow      = Texture( "../GameEngine/textures/snowrocks.png", "snow" );
    Texture rock      = Texture( "../GameEngine/textures/rock.png", "rock" );
    Texture grass     = Texture( "../GameEngine/textures/grass.png", "grass" );

    // Terrain Game Object --------------------------------------------------------------------------

    std::vector<Texture> terrainTextures;
    terrainTextures.push_back( heightMap );
    terrainTextures.push_back( snow );
    terrainTextures.push_back( rock );
    terrainTextures.push_back( grass );

    terrain = Terrain( heightMap );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white );

    terrainGO = new GameObject( "Terrain" );

    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, terrainGO->getTransform() );

    terrainGO->addComponent( terrainRenderer );

    // Player Game Object  -------------------------------------------------------------------------------


    std::vector<Texture> bunnyTextures;
    bunnyTextures.push_back( grass );

    Mesh playerMesh = Mesh( sphereObj ,bunnyTextures, shader, white );

    playerGO  = new GameObject( "Player", terrainGO );
    playerGO->scale( QVector3D( 0.01, 0.01, 0.01 ) );

    MeshRenderer * playerRenderer = new MeshRenderer( playerMesh, playerGO->getTransform() );

    playerGO->addComponent( playerRenderer );

    this->player = Player( *playerGO );
    this->player.setMesh( playerMesh );
    this->player.move( QVector3D(0.0, 0.0, 0.0), terrain );

    // Build hierarchy --------------------------------------------------------------------------

    terrainGO->addChild( playerGO );

    sceneGraph = SceneGraph( terrainGO );

}

void CoreEngine::paintGL()
{

    float currentFrame = time.elapsed()/1000.;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->sceneGraph.render( sceneGraph.getRoot(), camera , projection );

}
