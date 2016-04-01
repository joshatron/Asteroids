HEADERS += glwidget.h gameengine.h gamestate.h asteroid.h bullet.h object.h ship.h convex_shape.h stats.h collision.h animation.h main_ship.h star.h controller.h asteroid_controller.h hexa_ship.h
SOURCES += glwidget.cpp main.cpp gameengine.cpp collision.cpp main_ship.cpp ship.cpp bullet.cpp star.cpp animation.cpp asteroid.cpp asteroid_controller.cpp hexa_ship.cpp

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
