#-------------------------------------------------
#
# Project created by QtCreator 2021-06-10T06:59:31
#
#-------------------------------------------------

CONFIG  += core gui
QT      += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GLFWInterop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#WINDOW
LIBS += -lUser32 -lOpenGL32 -lGdi32 -lKernel32

#GLEW
INCLUDEPATH += $$PWD/glew-2.1.0/include
DEPENDPATH += $$PWD/glew-2.1.0/include

LIBS += -L$$PWD/glew-2.1.0/lib/Release/x64/ -lglew32

#GLFW
#INCLUDEPATH += $$PWD/glfw-3.3.4/include
#DEPENDPATH += $$PWD/glfw-3.3.4/include

#LIBS += -L$$PWD/glfw-3.3.4/lib/ -lglfw3dll

INCLUDEPATH += $$PWD/glfw-3/include
DEPENDPATH += $$PWD/glfw-3/include

LIBS += -L$$PWD/glfw-3/lib/ -lglfw3dll

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        openglrendererabstract.cpp \
        qtopenglrenderer.cpp \
        qtopenglrendersurface.cpp \
        qtopenglrenderwindow.cpp \
        glfwopenglrenderer.cpp \
        glfwopenglrenderwindow.cpp \
        qtopenglrenderwindownative.cpp \
    trianglerendererwindow.cpp

HEADERS += \
        mainwindow.h \
        openglrendererabstract.h \
        qtopenglrenderer.h \
        qtopenglrendersurface.h \
        qtopenglrenderwindow.h \
        glfwopenglrenderer.h \
        glfwopenglrenderwindow.h \
        qtopenglrenderwindownative.h \
    trianglerendererwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc
