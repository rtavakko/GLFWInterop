#ifndef QTOPENGLRENDERWINDOWNATIVE_H
#define QTOPENGLRENDERWINDOWNATIVE_H

#include <glfwopenglrenderwindow.h>

#include <qtopenglrendersurface.h>
#include <qtopenglrenderwindow.h>

#include <QObject>

#include <QOpenGLContext>
#include <QtPlatformHeaders/QWGLNativeContext>

class QTOpenGLRenderWindowNative : public QObject
{
    Q_OBJECT
public:
    explicit QTOpenGLRenderWindowNative(QTOpenGLRenderSurface* renderSurface);
    explicit QTOpenGLRenderWindowNative(QTOpenGLRenderWindow* renderSurface);

public slots:
    //This calls initialize and creates a native window; it must be called only after this object is moved to its intended thread
    virtual void create();

    //These are the main functions we will use
    virtual void resize(unsigned int w, unsigned int h);
    virtual void updateSpecs(GLFWOpenGLRenderer::OpenGLRenderSpecs specs);

    virtual void renderFrame(GLuint texID, unsigned int width, unsigned int height);

signals:

protected:
    //QT resources that we will use to create a wrapper for the native window and context
    QTOpenGLRenderWindow* renderWindow;

    QOpenGLContext* openGLContext;

    QOpenGLContext* sharedOpenGLContext;

    //Native render window implementation
    GLFWOpenGLRenderWindow* renderWindowNative;
};

#endif // QTOPENGLRENDERWINDOWNATIVE_H
