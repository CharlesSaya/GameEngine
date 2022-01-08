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

    Texture skyboxBottom = Texture( "../GameEngine/textures/skybox/Night/py.png", "skyboxBottom" );
    Texture skyboxTop    = Texture( "../GameEngine/textures/skybox/Night/ny.png", "skyboxTop"    );
    Texture skyboxRight  = Texture( "../GameEngine/textures/skybox/Night/px.png", "skyboxRight"  );
    Texture skyboxLeft   = Texture( "../GameEngine/textures/skybox/Night/nx.png", "skyboxLeft"   );
    Texture skyboxFront  = Texture( "../GameEngine/textures/skybox/Night/nz.png", "skyboxFront"  );
    Texture skyboxBack   = Texture( "../GameEngine/textures/skybox/Night/pz.png", "skyboxBack"   );

    std::vector<Texture> skyboxTextures = { skyboxRight, skyboxBottom, skyboxFront, skyboxLeft, skyboxTop, skyboxBack };
    cubemap = CubeMap( 15*64, skyboxShader, skyboxTextures );
    renderingEngine.setSkybox( cubemap );

    // Terrain Game Object ------------------------------------------------------------------------------

    Texture heightMap = Texture( "../GameEngine/textures/heightmapF8.png", "tex0" );

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

    terrain = Terrain( 128, 1.0, terrainOBJ, heightMap );
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

    playerGO  = new GameObjectPlayer( "Player" , playerRenderer, playerMove, playerPhysics, playerCollider, playerComponent,terrainGO );
    playerGO->scale(  QVector3D(0.1, 0.1, 0.1));
    playerGO->move( QVector3D(getHeightObject(26.0543,23.3017)));


    // Sphere
    std::vector<Texture> sphereTextures;
    sphereTextures.push_back( grass );

    Mesh sphereMesh = Mesh( sphereObj, sphereTextures, shader, white, false );
    MeshRenderer * sphereRenderer = new MeshRenderer( sphereMesh, this  );
    ColliderComponent * sphereCollider = new ColliderComponent( this );
    PhysicsComponent * spherePhysics = new PhysicsComponent(physicsEngine, this );

    sphereGO = new GameObjectMesh( "Sphere", sphereRenderer, sphereCollider,spherePhysics, true, terrainGO );
    sphereGO->scale( 1);
    sphereGO->move(  QVector3D(-0.0,0.0f,10.0f));
    sphereGO->setIsCollectible( true );

    this->goMeshes.push_back( sphereGO );

    // snowy tree  ------------------------------------------------------------------------------

    Texture treeSnow   = Texture( "../GameEngine/textures/T_Tree_winter", "snowTree" );

    std::vector<std::string> snowyObj;
    snowyObj.push_back("../GameEngine/objects/snowyFir/");
    snowyObj.push_back("../GameEngine/objects/snowyTree/");
    snowyObj.push_back("../GameEngine/objects/snowyStump/");
    snowyObj.push_back("../GameEngine/objects/snowyTreeV2/");

    std::vector<Texture> treeSnowTextures;
    treeSnowTextures.push_back( treeSnow );

    QVector<QVector3D> positionsTreeSnow;
    QVector<float> scalesTreeSnow;
    positionsTreeSnow.push_back(QVector3D(getHeightObject(21.122,73.92)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(13.122,87.92)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(8.86,105.344)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(8.86,105.344)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(12.6731,109.539)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(15.2169,104.007)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(34.2184,104.791)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(29.0421,100.255)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(31.921,96.8429)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(32.4277,94.3707)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(28.962,92.7587)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(27.0081,96.6063)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(23.1854,101.203)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(19.7761,105.718)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(20.8789,90.1217)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(26.7896,82.0665)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(33.8752,80.1888)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(27.4411,80.4065)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(37.9303,84.2533)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(32.045,73.5333)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(40.5250,75.423)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(50.2974,73.2955)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(52.786,78.846)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(63.8832,77.3402)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(67.4085,77.525)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(70.9541,82.8983)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(79.6318,79.0449)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(84.7804,78.2473)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(103.834,79.6695)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(107.366,82.7031)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(102.199,91.3177)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(103.96,98.252)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(109.881,99.48)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(111.997,103.45)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(109.883,97.95)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(106.387,98.1969)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(99.342,115.686)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(95.4537,118.464)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(89.3565,118.35)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(84.7227,119.266)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(80.7093,112.646)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(50.8224,107.928)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(37.2173,109.069)));
    positionsTreeSnow.push_back(QVector3D(getHeightObject(38.1351,98.8298)));



    for(int i = 0 ;i<positionsTreeSnow.size();i++ ){
        scalesTreeSnow.push_back( 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.6))));
    }

    for(int i = 0 ;i<positionsTreeSnow.size();i++ ){
        Mesh treeMesh =Mesh( snowyObj[rand()%snowyObj.size()], treeSnowTextures, shader, white, false );
        GameObjectMesh * treeGO = new GameObjectMesh( "Tree" + std::to_string(i), new MeshRenderer( treeMesh, this  ), new ColliderComponent( this ), false,terrainGO );
        treeGO->scale( scalesTreeSnow[i] );
        treeGO->move( positionsTreeSnow[i] );
//        treeGO->getColliderComponent()->
        this->goMeshes.push_back( treeGO );
    }

    // cactus  ------------------------------------------------------------------------------


    std::vector<std::string> cactusObj;
    cactusObj.push_back("../GameEngine/objects/desertCactus/");
    cactusObj.push_back("../GameEngine/objects/desertCactusV2/");
    cactusObj.push_back("../GameEngine/objects/desertCactusV3/");

    Texture cactusTexture   = Texture( "../GameEngine/textures/cactusTexture", "cactusTexture" );
    std::vector<Texture> cactusTextures;
    cactusTextures.push_back(cactusTexture  );

    QVector<QVector3D> positionsCactus;
    QVector<float> scalesCactus;

    positionsCactus.push_back(QVector3D(getHeightObject(25.9588,53.2657)));
    positionsCactus.push_back(QVector3D(getHeightObject(17.2045,47.50)));
    positionsCactus.push_back(QVector3D(getHeightObject(35.3655,50.5533)));
    positionsCactus.push_back(QVector3D(getHeightObject(44.281,41.5533)));
    positionsCactus.push_back(QVector3D(getHeightObject(39.5726,44.8162)));
    positionsCactus.push_back(QVector3D(getHeightObject(51.0103,41.7781)));
    positionsCactus.push_back(QVector3D(getHeightObject(20.6871,44.7781)));
    positionsCactus.push_back(QVector3D(getHeightObject(48.8608,55.7303)));
    positionsCactus.push_back(QVector3D(getHeightObject(64.1906,55.9789)));

    positionsCactus.push_back(QVector3D(getHeightObject(90.9981,51.3017)));
    positionsCactus.push_back(QVector3D(getHeightObject(106.77,59.752)));
    positionsCactus.push_back(QVector3D(getHeightObject(98.2764,43.2881)));
    positionsCactus.push_back(QVector3D(getHeightObject(80.4497,46.281)));
    positionsCactus.push_back(QVector3D(getHeightObject(73.4896,50.)));


    for(int i = 0 ;i<positionsCactus.size();i++ ){
        scalesCactus.push_back( 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.7))));
    }

    for(int i = 0 ;i<positionsCactus.size();i++ ){
        Mesh cactusMesh =Mesh( cactusObj[rand()%cactusObj.size()], cactusTextures, shader, white, false );
        GameObjectMesh * cactusGO = new GameObjectMesh( "cactus" + std::to_string(i), new MeshRenderer( cactusMesh, this  ), new ColliderComponent( this ), false, terrainGO );
        cactusGO->scale( scalesCactus[i] );
        cactusGO->move( positionsCactus[i] );
        this->goMeshes.push_back( cactusGO );
    }

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

        positionsBox.push_back(QVector3D(getHeightObject(30.2537,23.5112)));
        positionsBox.push_back(QVector3D(getHeightObject(16.169,14.781 )));
        positionsBox.push_back(QVector3D(getHeightObject(14.2261,25.9705 )));
        positionsBox.push_back(QVector3D(getHeightObject(31.0522,34.9705 )));
        positionsBox.push_back(QVector3D(getHeightObject(50.2872,16.6526 )));
        positionsBox.push_back(QVector3D(getHeightObject(113.914,29.2116 )));
        positionsBox.push_back(QVector3D(getHeightObject(95.8851,46.9473 )));
        positionsBox.push_back(QVector3D(getHeightObject(74.2979,47.5831 )));
        positionsBox.push_back(QVector3D(getHeightObject(74.2979,47.5831 )));
        positionsBox.push_back(QVector3D(getHeightObject(112.439,45.2934 )));
        positionsBox.push_back(QVector3D(getHeightObject(28.8756,84.9065 )));
        positionsBox.push_back(QVector3D(getHeightObject(65.6861,84.8065 )));
        positionsBox.push_back(QVector3D(getHeightObject(93.6861,98.8065 )));
        positionsBox.push_back(QVector3D(getHeightObject(96.608,117.022 )));

        for(int i = 0 ;i<positionsBox.size();i++ ){
             scalesBox.push_back( 0.01 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5))));

        }

        for(int i = 0 ;i<positionsBox.size();i++ ){
            int randomObj = rand()%boxObj.size();
            Mesh boxMesh =Mesh( boxObj[randomObj], listTexturesBox[randomObj], shader, white, false );
            GameObjectMesh * boxGO = new GameObjectMesh( "box" + std::to_string(i), new MeshRenderer( boxMesh, this  ), new ColliderComponent( this ), new PhysicsComponent(physicsEngine, this ) ,true, terrainGO );
            boxGO->scale( scalesBox[i] );
            boxGO->move( positionsBox[i] );
            this->goMeshes.push_back( boxGO );
        }

    // Collectible ------------------------------------------------------------------------------

        // bush  ------------------------------------------------------------------------------

        Texture bushTexture   = Texture( "../GameEngine/textures/bushTexture", "bushTexture" );

        std::vector<std::string> bushObj;
        bushObj.push_back("../GameEngine/objects/smallBush/");
        bushObj.push_back("../GameEngine/objects/bigBush/");


        std::vector<Texture> bushTextures;
        bushTextures.push_back( bushTexture );

        QVector<QVector3D> positionsBush;
        QVector<float> scalesBush;
        positionsBush.push_back(QVector3D(getHeightObject(33.1903,16.271 )));
        positionsBush.push_back(QVector3D(getHeightObject(45.238,25.0801  )));
        positionsBush.push_back(QVector3D(getHeightObject(57.1722,12.107  )));
        positionsBush.push_back(QVector3D(getHeightObject(70.7692,15.9081  )));
        positionsBush.push_back(QVector3D(getHeightObject(89.2729,26.8545  )));
        positionsBush.push_back(QVector3D(getHeightObject(111.59,18.6222  )));

        for(int i = 0 ;i<positionsBush.size();i++ ){
            scalesBush.push_back( 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.6))));
        }

        for(int i = 0 ;i<positionsBush.size();i++ ){
            Mesh bushMesh =Mesh( bushObj[rand()%bushObj.size()], bushTextures, shader, white, false );
            GameObjectMesh * bushGO = new GameObjectMesh( "Bush" + std::to_string(i), new MeshRenderer( bushMesh, this  ), new ColliderComponent( this ), false ,terrainGO);
            bushGO->scale( scalesBush[i] );
            bushGO->move( positionsBush[i] );

            this->goMeshes.push_back( bushGO );
        }


        // tree   ------------------------------------------------------------------------------

        Texture treeTexture   = Texture( "../GameEngine/textures/TreeTexture", "treeTexture" );

        std::vector<std::string> treeObj;
        treeObj.push_back("../GameEngine/objects/BigTreeWithLeaves/");
        treeObj.push_back("../GameEngine/objects/SmallTreeWithLeave/");
        treeObj.push_back("../GameEngine/objects/TreeNoLeavesBig/");
        treeObj.push_back("../GameEngine/objects/TreeNoLeavesSmall/");



        std::vector<Texture> treeTextures;
        treeTextures.push_back( treeTexture );

        QVector<QVector3D> positionsTree;
        QVector<float> scalesTree;
        positionsTree.push_back(QVector3D(getHeightObject(36.4723,21.4726 )));
        positionsTree.push_back(QVector3D(getHeightObject(44.6209,17.2689 )));
        positionsTree.push_back(QVector3D(getHeightObject(69.8517,5.18626 )));
        positionsTree.push_back(QVector3D(getHeightObject(84.5685,16.0831 )));
        positionsTree.push_back(QVector3D(getHeightObject(99.8933,25.50)));
        positionsTree.push_back(QVector3D(getHeightObject(119.313,26.5568)));
        positionsTree.push_back(QVector3D(getHeightObject(55.313,21.8187)));
        positionsTree.push_back(QVector3D(getHeightObject(77.95,23.4315)));
        positionsTree.push_back(QVector3D(getHeightObject(99.94,15.5421)));
        positionsTree.push_back(QVector3D(getHeightObject(66.115,27.0250)));
        positionsTree.push_back(QVector3D(getHeightObject(94.7229,4.2864)));
        positionsTree.push_back(QVector3D(getHeightObject(9.01008,16.3732)));


        for(int i = 0 ;i<positionsTree.size();i++ ){
            scalesTree.push_back( 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.6))));
        }


        for(int i = 0 ;i<positionsTree.size();i++ ){
            scalesTree.push_back( 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.7))));
        }

        for(int i = 0 ;i<positionsTree.size();i++ ){
            Mesh treeMesh =Mesh( treeObj[rand()%treeObj.size()], treeTextures, shader, white, false );
            GameObjectMesh * treeGO = new GameObjectMesh( "Tree" + std::to_string(i), new MeshRenderer( treeMesh, this  ), new ColliderComponent( this ), false ,terrainGO);
            treeGO->scale( scalesTree[i] );
            treeGO->move( positionsTree[i] );

            this->goMeshes.push_back( treeGO );
        }


//    treeHouse  ------------------------------------------------------------------------------

        string treeHouseObj;
        treeHouseObj= "../GameEngine/objects/treeHouse/";


        Texture treeHouseTexture   = Texture( "../GameEngine/textures/treeHouseTexture", "treeHouseTexture" );
        std::vector<Texture> treeHouseTextures;
        treeHouseTextures.push_back(treeHouseTexture  );

        Mesh treeHouseMesh =Mesh(treeHouseObj, treeHouseTextures, shader, white, false );
        GameObjectMesh * treeHouseGO = new GameObjectMesh( "treeHouse", new MeshRenderer( treeHouseMesh, this  ), new ColliderComponent( this ), new PhysicsComponent(physicsEngine, this ) ,true, terrainGO );
        treeHouseGO->scale( 0.05);
//        treeHouseGO->rotate(QVector3D(1.0f,0.0f,0.0f),-90) ;
//        treeHouseGO->rotate(QVector3D(0.0f,0.0f,1.0f),90) ;
        treeHouseGO->move( getHeightObject(16.1342,31.3343 ));
        this->goMeshes.push_back( treeHouseGO );

        // bush  ------------------------------------------------------------------------------

        Texture gridTexture   = Texture( "../GameEngine/textures/rockTexture", "doorTexture" );

        string gridObj;
        gridObj = "../GameEngine/objects/grid/";

        std::vector<Texture> gridTextures;
        gridTextures.push_back( gridTexture );

        QVector<QVector3D> positionsGrid;
        QVector<QVector3D> scalesGrid;
        positionsGrid.push_back(QVector3D(getHeightObject(98.2476,36.6745 )));
        positionsGrid.push_back(QVector3D(getHeightObject(24.278,66.278 )));
        positionsGrid.push_back(QVector3D(getHeightObject(88.8245,119.832 )));

        for(int i = 0 ;i<positionsGrid.size()-1;i++ ){
            scalesGrid.push_back( QVector3D(10.0,10.0f,1.0f));
        }
        scalesGrid.push_back( QVector3D(20.0,15.0f,1.0f));
        for(int i = 0 ;i<positionsGrid.size();i++ ){
            Mesh gridMesh =Mesh( gridObj, gridTextures, shader, white, false );
            GameObjectMesh * gridGO = new GameObjectMesh( "Grid" + std::to_string(i), new MeshRenderer( gridMesh, this  ), new ColliderComponent( this ), false );
            gridGO->scale( scalesGrid[i] );
            gridGO->move( positionsGrid[i] );

            this->goMeshes.push_back( gridGO );
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

    sceneGraph = SceneGraph( baseGo, this->goMeshes, playerGO, mainCameraGO, this->physicsEngine, this->colliderEngine, terrain );
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

QVector3D Game::getHeightObject(float x, float z)
{
    QVector3D vectorXZ = QVector3D(x,0.0,z);
    return QVector3D(x,terrain.getHeightTerrain(vectorXZ ),z);
}

