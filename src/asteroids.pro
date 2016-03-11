HEADERS += glwidget.h gameengine.h gamestate.h asteroid.h bullet.h object.h ship.h convex_shape.h stats.h collision.h animation.h
SOURCES += glwidget.cpp main.cpp gameengine.cpp collision.cpp

macx {
	QMAKE_MAC_SDK = macosx10.11
}

QT += opengl
CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11
INCLUDEPATH += "../include"

RESOURCES += \
    shaders.qrc
