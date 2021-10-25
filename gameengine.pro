QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += src/main.cpp \
    src/gameObject.cpp \
    src/mesh.cpp \
    src/sceneGraph.cpp \
    src/transform.cpp \
    src/mainwidget.cpp \
    src/geometryengine.cpp

HEADERS += \
    headers/BasicIO.h \
    headers/camera.h \
    headers/gameObject.h \
    headers/mainwidget.h \
    headers/geometryengine.h \
    headers/mesh.h \
    headers/sceneGraph.h \
    headers/transform.h

RESOURCES += \
    objects.qrc \
    shaders.qrc \
    textures.qrc \

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES +=
