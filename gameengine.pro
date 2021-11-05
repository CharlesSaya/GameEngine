QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += src/game/main.cpp \
    src/core/coreEngine.cpp \
    src/core/gameobjectcamera.cpp \
    src/core/gameobjectmesh.cpp \
    src/game/game.cpp \
    src/render/AABB.cpp \
    src/core/gameObject.cpp \
    src/render/BasicShader.cpp \
    src/render/mesh.cpp \
    src/core/meshrenderer.cpp \
    src/game/sceneGraph.cpp \
    src/core/transform.cpp \
    src/render/geometryengine.cpp \
    src/render/renderingEngine.cpp \
    src/physics/physicsEngine.cpp \


HEADERS += \
    headers/core/coreEngine.h \
    headers/core/gameobjectcamera.h \
    headers/core/gameobjectmesh.h \
    headers/game/game.h \
    headers/render/AABB.h \
    headers/core/BasicIO.h \
    headers/render/BasicShader.h \
    headers/render/camera.h \
    headers/core/gameComponent.h \
    headers/core/gameObject.h \
    headers/render/geometryengine.h \
    headers/render/mesh.h \
    headers/core/meshrenderer.h \
    headers/game/sceneGraph.h \
    headers/core/transform.h \
    headers/render/renderingEngine.h \
    headers/physics/physicsEngine.h \

RESOURCES += \
    objects.qrc \
    shaders.qrc \
    textures.qrc \

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES +=
