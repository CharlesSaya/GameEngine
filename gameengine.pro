QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    gameObject.cpp \
    gameobjectcamera.cpp \
    gameobjectmesh.cpp \
    mesh.cpp \
    sceneGraph.cpp \
    transform.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    BasicIO.h \
    camera.h \
    gameObject.h \
    gameobjectcamera.h \
    gameobjectmesh.h \
    mainwidget.h \
    geometryengine.h \
    mesh.h \
    sceneGraph.h \
    transform.h

RESOURCES += \
    objects.qrc \
    shaders.qrc \
    textures.qrc \

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES +=
