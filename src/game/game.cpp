#include "headers/game/game.h"

Game::Game( QObject * parent ) : QObject(parent){

}

Game::Game( CameraComponent * camera,  QObject * parent ) : QObject(parent){
    this->camera = camera;
}

void Game::initGame(){

    // Shaders & Lights  --------------------------------------------------------------------------------

    shader = new Shader( "../GameEngine/shaders/base_vshader.glsl", "../GameEngine/shaders/base_fshader.glsl" );
    terrainShader = new Shader(  "../GameEngine/shaders/terrain_vshader.glsl", "../GameEngine/shaders/terrain_fshader.glsl" );
    skyboxShader = new Shader(  "../GameEngine/shaders/skybox_vshader.glsl", "../GameEngine/shaders/skybox_fshader.glsl" );

    // Build scene graph  -------------------------------------------------------------------------------

    std::string sphereObj = "../GameEngine/objects/sphere/";
    std::string bunnyObj  = "../GameEngine/objects/bunny/";
    std::string terrainOBJ  = "../GameEngine/objects/terrain/";


    // Environment ------------------------------------------------------------------------------

    Texture skyboxBottom = Texture( "../GameEngine/textures/skybox/MusicHall/py.png", "skyboxBottom" );
    Texture skyboxTop    = Texture( "../GameEngine/textures/skybox/MusicHall/ny.png", "skyboxTop"    );
    Texture skyboxRight  = Texture( "../GameEngine/textures/skybox/MusicHall/px.png", "skyboxRight"  );
    Texture skyboxLeft   = Texture( "../GameEngine/textures/skybox/MusicHall/nx.png", "skyboxLeft"   );
    Texture skyboxFront  = Texture( "../GameEngine/textures/skybox/MusicHall/nz.png", "skyboxFront"  );
    Texture skyboxBack   = Texture( "../GameEngine/textures/skybox/MusicHall/pz.png", "skyboxBack"   );

    std::vector<Texture> skyboxTextures = { skyboxRight, skyboxBottom, skyboxFront, skyboxLeft, skyboxTop, skyboxBack };
    cubemap = CubeMap( 15*64, skyboxShader, skyboxTextures );
    renderingEngine.setSkybox( cubemap );

    // Terrain Game Object ------------------------------------------------------------------------------

    Texture heightMap = Texture( "../GameEngine/textures/heightmapF3.png", "tex0" );

    Texture snow    = Texture( "../GameEngine/textures/snowTexture.png", "tex0" );
    Texture rock    = Texture( "../GameEngine/textures/rockTexture.png", "tex1" );
    Texture grass   = Texture( "../GameEngine/textures/grassTexture.png", "tex2" );
    Texture plateau = Texture( "../GameEngine/textures/plateauTexture.png", "tex3" );
    Texture sand    = Texture( "../GameEngine/textures/sandTexture.png", "tex4" );
    Texture hill    = Texture( "../GameEngine/textures/hillTexture.png", "tex5" );

    Texture blendMapTop = Texture( "../GameEngine/textures/blendmapTop.png", "blendmapTop" );
    Texture blendMapMiddle = Texture( "../GameEngine/textures/blendmapMiddle.png", "blendmapMiddle" );
    Texture blendMapBottom = Texture( "../GameEngine/textures/blendmapBottom.png", "blendmapBottom" );

    std::vector<Texture> terrainTextures;

    terrainTextures.push_back( snow );
    terrainTextures.push_back( rock );
    terrainTextures.push_back( grass );
    terrainTextures.push_back( plateau );
    terrainTextures.push_back( sand );
    terrainTextures.push_back( hill );

    terrainTextures.push_back( blendMapTop );
    terrainTextures.push_back( blendMapMiddle );
    terrainTextures.push_back( blendMapBottom );

    terrain = Terrain( 256, 1.0, terrainOBJ, heightMap );
    Mesh terrainMesh = Mesh( terrain, terrainTextures, terrainShader, white, false );
    MeshRenderer * terrainRenderer = new MeshRenderer( terrainMesh, this );
    ColliderComponent * terrainCollider = new ColliderComponent( this );

    terrainGO = new GameObjectMesh( "Terrain", terrainRenderer, terrainCollider, false );
    terrainGO->scale( terrain.getScale() );
//    terrainGO->move( QVector3D( -64.0 * terrain.getScale(), 0.0, 64.0 * terrain.getScale() ) );

    this->goMeshes.push_back( terrainGO );
    colliderEngine.setTerrain( terrain );

    // Player Game Object  ------------------------------------------------------------------------------

    Texture sandP      = Texture( "../GameEngine/textures/sandTexture.png", "tex0" );
    Texture grassP      = Texture( "../GameEngine/textures/snowrocks.png", "tex1" );

    std::vector<Texture> playerTextures;
    playerTextures.push_back( sandP );
    playerTextures.push_back( grassP );

    Mesh playerMesh = Mesh( bunnyObj, playerTextures, shader, white, false );

    MeshRenderer * playerRenderer      = new MeshRenderer( playerMesh, this );
    MoveComponent * playerMove         = new MoveComponent( terrain, this );
    ColliderComponent * playerCollider = new ColliderComponent( this );
    PlayerComponent * playerComponent  = new PlayerComponent( shader, this );
    PhysicsComponent * playerPhysics   = new PhysicsComponent( physicsEngine, this );

    connect( this, &Game::sendPressedKey, playerMove, &MoveComponent::pressedInput );
    connect( this, &Game::sendReleasedKey, playerMove, &MoveComponent::releasedInput );
    connect( this, &Game::sendMouseMoved, playerMove, &MoveComponent::mouseMoveEvent );

    connect( this, &Game::sendPressedMouse, playerComponent, &PlayerComponent::pressedInput );
    connect( this, &Game::sendreleasedMouse, playerComponent, &PlayerComponent::releasedInput );
    connect( this, &Game::sendMouseWheel, playerComponent, &PlayerComponent::wheelScrolled );

    playerGO  = new GameObjectPlayer( "Player" , playerRenderer, playerMove, playerPhysics, playerCollider, playerComponent );
    playerGO->scale(  QVector3D(0.1, 0.1, 0.1) );
    playerGO->move(  QVector3D(10., 30., -10. ) );


    // Sphere
    std::vector<Texture> sphereTextures;
    sphereTextures.push_back( grass );

    Mesh sphereMesh = Mesh( sphereObj, sphereTextures, shader, white, false );
    MeshRenderer * sphereRenderer = new MeshRenderer( sphereMesh, this  );
    ColliderComponent * sphereCollider = new ColliderComponent( this );
    PhysicsComponent * spherePhysics = new PhysicsComponent(physicsEngine, this );

    sphereGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider,spherePhysics, true, terrainGO );
    sphereGO->scale( 1 );
    sphereGO->move(  QVector3D(-103.6432,0.5f,73.0f));

//    this->goMeshes.push_back( sphereGO );

    // snowy tree  ------------------------------------------------------------------------------

    Texture treeSnow   = Texture( "../GameEngine/textures/T_Tree_winter", "snowTree" );

    std::vector<std::string> snowyObj;
    snowyObj.push_back("../GameEngine/objects/snowyFir/");
    snowyObj.push_back("../GameEngine/objects/snowyTree/");
    snowyObj.push_back("../GameEngine/objects/snowyStump/");
    snowyObj.push_back("../GameEngine/objects/snowyTreeV2/");

    std::vector<Texture> treeTextures;
    treeTextures.push_back( treeSnow );

    QVector<QVector3D> positionsTree;
    QVector<float> scalesTree;

    positionsTree.push_back(QVector3D(-46.6432,0.0f,43.0f));
    positionsTree.push_back(QVector3D(-17.0,0.0f,51.0f));
    positionsTree.push_back(QVector3D(-11.0,0.0f,42.0f));
    positionsTree.push_back(QVector3D(-60.0,0.0f,15.0f));
    positionsTree.push_back(QVector3D(-120.0,0.0f,20.0f));
    positionsTree.push_back(QVector3D(-117.0,0.0f,29.0f));
    positionsTree.push_back(QVector3D(-110.0,0.0f,56.0f));
    positionsTree.push_back(QVector3D(-89.0,0.0f,53.0f));
    positionsTree.push_back(QVector3D(-72.0,0.0f,48.0f));
    positionsTree.push_back(QVector3D(-62.0,0.0f,50.0f));
    positionsTree.push_back(QVector3D(-47.0,0.0f,19.0f));
    positionsTree.push_back(QVector3D(-45.0,0.0f,9.0f));
    positionsTree.push_back(QVector3D(-67.0,0.0f,22.0f));
    positionsTree.push_back(QVector3D(-60.0,0.0f,26.0f));
    positionsTree.push_back(QVector3D(-21.0,0.0f,46.0f));
    positionsTree.push_back(QVector3D(-13.0,0.0f,43.0f));
    positionsTree.push_back(QVector3D(-9.0,0.0f,19.0f));
    positionsTree.push_back(QVector3D(-30.0,0.0f,8.0f));
    positionsTree.push_back(QVector3D(-41.0,0.0f,15.0f));
    positionsTree.push_back(QVector3D(-98.0,0.0f,32.0f));
    positionsTree.push_back(QVector3D(-84.0,0.0f,35.0f));
    positionsTree.push_back(QVector3D(-99.0,0.0f,40.0f));

    for(int i = 0 ;i<positionsTree.size();i++ ){
        scalesTree.push_back( 0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5))));
    }

    for(int i = 0 ;i<positionsTree.size();i++ ){
        Mesh treeMesh =Mesh( snowyObj[rand()%snowyObj.size()], treeTextures, shader, white, false );
        GameObjectMesh * treeGO = new GameObjectMesh( "Tree" + std::to_string(i), new MeshRenderer( treeMesh, this  ), new ColliderComponent( this ), false, terrainGO );
        treeGO->scale( scalesTree[i] );
        treeGO->move( positionsTree[i] );
        this->goMeshes.push_back( treeGO );
    }

    // cactus  ------------------------------------------------------------------------------


//    std::vector<std::string> cactusObj;
//    cactusObj.push_back("../GameEngine/objects/desertCactus/");
//    cactusObj.push_back("../GameEngine/objects/desertCactusV2/");
//    cactusObj.push_back("../GameEngine/objects/desertCactusV3/");

//    Texture cactusTexture   = Texture( "../GameEngine/textures/cactusTexture", "cactusTexture" );
//    std::vector<Texture> cactusTextures;
//    cactusTextures.push_back(cactusTexture  );

//    QVector<QVector3D> positionsCactus;
//    QVector<float> scalesCactus;

//    positionsCactus.push_back(QVector3D(-100.6432,0.5f,73.0f));
////    positionsCactus.push_back(QVector3D(-100.6432,0.0f,73.0f));

//    for(int i = 0 ;i<positionsCactus.size();i++ ){
//        scalesCactus.push_back( 0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5))));
//    }

//    for(int i = 0 ;i<positionsCactus.size();i++ ){
//        Mesh cactusMesh =Mesh( cactusObj[rand()%cactusObj.size()], cactusTextures, shader, white, false );
//        GameObjectMesh * cactusGO = new GameObjectMesh( "cactus" + std::to_string(i), new MeshRenderer( cactusMesh, this  ), new ColliderComponent( this ), false, terrainGO );
//        cactusGO->scale( scalesCactus[i] );
//        cactusGO->move( positionsCactus[i] );
//        this->goMeshes.push_back( cactusGO );
//    }

    //    woodBox  ------------------------------------------------------------------------------


        std::vector<std::string> boxObj;
        boxObj.push_back("../GameEngine/objects/woodBox/");

        Texture boxTexture   = Texture( "../GameEngine/textures/textureBox", "textureBox" );
        std::vector<std::vector<Texture>> listTexturesBox;
        std::vector<Texture> boxTextures;

        boxTextures.push_back(boxTexture  );
        listTexturesBox.push_back(boxTextures);
        QVector<QVector3D> positionsBox;
        QVector<float> scalesBox;

        positionsBox.push_back(QVector3D(-100.6432,0.5f,73.0f));
        positionsBox.push_back(QVector3D(-90.6432,0.5f,73.0f));
        positionsBox.push_back(QVector3D(-80.6432,0.5f,73.0f));
        positionsBox.push_back(QVector3D(-70.6432,0.5f,73.0f));
    //    positionsCactus.push_back(QVector3D(-100.6432,0.0f,73.0f));

        for(int i = 0 ;i<positionsBox.size();i++ ){
            scalesBox.push_back(((rand()%10))/10.0f);
//            qDebug()<<scalesBox[i];
        }

        for(int i = 0 ;i<positionsBox.size();i++ ){
            int randomObj = rand()%boxObj.size();
            Mesh boxMesh =Mesh( boxObj[randomObj], listTexturesBox[randomObj], shader, white, false );
            GameObjectMesh * boxGO = new GameObjectMesh( "box" + std::to_string(i), new MeshRenderer( boxMesh, this  ), new ColliderComponent( this ), new PhysicsComponent(physicsEngine, this ) ,true, terrainGO );
            boxGO->scale( scalesBox[i] );
            boxGO->move( positionsBox[i] );
            this->goMeshes.push_back( boxGO );
        }




    // Camera  -------------------------------------------------------------------------------

    QVector3D cameraPosition = QVector3D();
    QVector3D cameraTarget   = playerGO->getWorldPosition();

    camera = new CameraComponent( cameraPosition, cameraTarget, fov, zNear, zFar );
    MoveComponent * cameraMove = new MoveComponent( terrain, this );
    PhysicsComponent * cameraPhysics = new PhysicsComponent( physicsEngine, this );
    ColliderComponent * cameraCollider = new ColliderComponent( this );

    connect( this, &Game::sendMouseMoved, cameraMove, &MoveComponent::mouseMoveEvent );

    mainCameraGO = new GameObjectCamera("Main camera",camera,cameraMove,cameraPhysics,cameraCollider, playerGO  );
    mainCameraGO->move(0.0f,0.0f,2.0f);
    mainCameraGO->updateCameraPosition();

    renderingEngine.setMainCamera( mainCameraGO );

    // Build hierarchy ---------------------------------------------------------------------------------
    std::vector<GameObject *> baseGo = { terrainGO, playerGO };

    sceneGraph = SceneGraph( baseGo, this->goMeshes, playerGO, mainCameraGO, this->physicsEngine, this->colliderEngine );
}

void Game::input( QKeyEvent * key  ){
    this->sceneGraph.input( key );
}

void Game::update( float fixedStep )
{
    this->sceneGraph.update( fixedStep  );
}

void Game::render(  float deltaTime ){
    renderingEngine.renderScene( this->sceneGraph, deltaTime );
}

// SLOTS
void Game::keyPressed( QKeyEvent * key ){
    emit this->sendPressedKey( key ) ;
}

void Game::keyReleased( QKeyEvent * key ){
    if( key->isAutoRepeat() )
        key->ignore();
    else
        emit this->sendReleasedKey( key ) ;
}

void Game::pressedMouse( QMouseEvent * event ){
    emit this->sendPressedMouse( event );
}

void Game::releasedMouse( QMouseEvent * event ){

    emit this->sendreleasedMouse( event );

}

void Game::mouseWheel( QWheelEvent * event ){
    emit this->sendMouseWheel( event );

}

void Game::mouseMoved( QVector2D pos, bool reset ){
    emit this->sendMouseMoved(pos, reset )  ;
}
// Getters & Setters

CameraComponent *Game::getCamera() const{
    return camera;
}

void Game::setCamera(CameraComponent *newCamera){
    camera = newCamera;
}

const PhysicsEngine &Game::getPhysicsEngine() const{
    return physicsEngine;
}

void Game::setPhysicsEngine(const PhysicsEngine &newPhysicsEngine){

    physicsEngine = newPhysicsEngine;

}

void Game::setProjection( float aspect ){
    this->camera->setProjectionPersp(aspect );
}

const ColliderEngine &Game::getColliderEngine() const
{
    return colliderEngine;
}

void Game::setColliderEngine(const ColliderEngine &newColliderEngine)
{
    colliderEngine = newColliderEngine;
}

const RenderingEngine &Game::getRenderingEngine() const
{
    return renderingEngine;
}

void Game::setRenderingEngine(const RenderingEngine &newRenderingEngine)
{
    renderingEngine = newRenderingEngine;
}
