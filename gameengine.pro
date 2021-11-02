QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += src/game/main.cpp \
    src/core/gameobjectcamera.cpp \
    src/core/gameobjectmesh.cpp \
    src/render/AABB.cpp \
    src/core/gameComponent.cpp \
    src/core/gameObject.cpp \
    src/render/mesh.cpp \
    src/core/meshrenderer.cpp \
    src/game/sceneGraph.cpp \
    src/core/transform.cpp \
    src/game/mainwidget.cpp \
    src/render/geometryengine.cpp

HEADERS += \
    headers/core/gameobjectcamera.h \
    headers/core/gameobjectmesh.h \
    headers/render/AABB.h \
    headers/core/BasicIO.h \
    headers/render/camera.h \
    headers/core/gameComponent.h \
    headers/core/gameObject.h \
    headers/game/mainwidget.h \
    headers/render/geometryengine.h \
    headers/render/mesh.h \
    headers/core/meshrenderer.h \
    headers/game/sceneGraph.h \
    headers/core/transform.h

RESOURCES += \
    objects.qrc \
    shaders.qrc \
    textures.qrc \

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES +=
