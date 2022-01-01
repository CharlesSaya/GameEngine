QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += src/game/main.cpp \
    src/core/cameraComponent.cpp \
    src/core/colliderComponent.cpp \
    src/core/coreEngine.cpp \
    src/core/gameObjectCamera.cpp \
    src/core/gameObjectMesh.cpp \
    src/core/gameObjectPlayer.cpp \
    src/core/moveComponent.cpp \
    src/core/objLoader.cpp \
    src/core/physicsComponent.cpp \
    src/core/player.cpp \
    src/core/playerComponent.cpp \
    src/game/MainWindow.cpp \
    src/game/game.cpp \
    src/physics/colliderEngine.cpp \
    src/render/AABB.cpp \
    src/core/gameObject.cpp \
    src/render/cubemap.cpp \
    src/render/directionalLight.cpp \
    src/render/light.cpp \
    src/render/mesh.cpp \
    src/core/meshrenderer.cpp \
    src/game/sceneGraph.cpp \
    src/core/transform.cpp \
    src/render/particleGenerator.cpp \
    src/render/pointLight.cpp \
    src/render/ray.cpp \
    src/render/renderingEngine.cpp \
    src/physics/physicsEngine.cpp \
    src/render/shader.cpp \
    src/render/terrain.cpp \
    src/render/texture.cpp


HEADERS += \
    headers/core/cameraComponent.h \
    headers/core/colliderComponent.h \
    headers/core/coreEngine.h \
    headers/core/gameObjectCamera.h \
    headers/core/gameObjectMesh.h \
    headers/core/gameObjectPlayer.h \
    headers/core/moveComponent.h \
    headers/core/objloader.h \
    headers/core/physicsComponent.h \
    headers/core/player.h \
    headers/core/playerComponent.h \
    headers/dirent.h \
    headers/game/MainWindow.h \
    headers/game/game.h \
    headers/game/node.h \
    headers/physics/colliderEngine.h \
    headers/render/AABB.h \
    headers/core/BasicIO.h \
    headers/core/gameComponent.h \
    headers/core/gameObject.h \
    headers/render/cubemap.h \
    headers/render/directionalLight.h \
    headers/render/light.h \
    headers/render/mesh.h \
    headers/render/mesh.h \
    headers/core/meshrenderer.h \
    headers/game/sceneGraph.h \
    headers/core/transform.h \
    headers/render/particleGenerator.h \
    headers/render/pointLight.h \
    headers/render/ray.h \
    headers/render/renderingEngine.h \
    headers/physics/physicsEngine.h \
    headers/render/shader.h \
    headers/render/spotLight.h \
    headers/render/terrain.h \
    headers/render/texture.h \
    headers/render/vertexData.h

RESOURCES += \
    objects.qrc \
    shaders.qrc \
    textures.qrc \

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES +=

