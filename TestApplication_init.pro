TEMPLATE = app

DEFINES += QT_DLL

CONFIG +=  debug_and_release

QT += core gui widgets openglwidgets

INCLUDEPATH += $$PWD/Source/Gui

SOURCES += Source/App/main.cpp
SOURCES += Source/Data/Data.cpp
SOURCES += Source/Data/Mesh.cpp
SOURCES += Source/Gui/GLWidget.cpp
SOURCES += Source/Gui/MainWidget.cpp

HEADERS += Source/Data/Data.h
HEADERS += Source/Data/Mesh.h
HEADERS += Source/Gui/GLWidget.h
HEADERS += Source/Gui/MainWidget.h

FORMS += Source/Gui/MainWidget.ui

win32:LIBS += -lopengl32
