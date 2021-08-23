#include "qtopenglrenderwindownative.h"

QTOpenGLRenderWindowNative::QTOpenGLRenderWindowNative(QTOpenGLRenderSurface *renderSurface) :
    QObject(nullptr),
    renderWindow(new QTOpenGLRenderWindow(nullptr,renderSurface->getSpecs(),renderSurface->getOpenGLFormat(),renderSurface->getOpenGLContext())),
    renderWindowNative(new GLFWOpenGLRenderWindow(renderWindow->getSpecs(),renderSurface->getOpenGLFormat(),renderWindow->winId())),
    openGLContext(nullptr),
    sharedOpenGLContext(renderWindow->getOpenGLContext())
{
    create();
    renderWindow->hide();
}

QTOpenGLRenderWindowNative::QTOpenGLRenderWindowNative(QTOpenGLRenderWindow *renderSurface) :
    QObject(nullptr),
    renderWindow(renderSurface),
    renderWindowNative(new GLFWOpenGLRenderWindow(renderWindow->getSpecs(),renderWindow->getOpenGLFormat(),renderWindow->winId())),
    openGLContext(nullptr),
    sharedOpenGLContext(renderWindow->getOpenGLContext())
{
    create();
    renderWindow->hide();
}

void QTOpenGLRenderWindowNative::create()
{
    //
    //This is where the "magic" happens for this project; in reality its where the atrocity happens...this stuff is fucking horrendous
    //

    //Allocate memory for the native context object and prepare to create it
    openGLContext = new QOpenGLContext(this);

    HGLRC hglrc = (HGLRC)(renderWindowNative->getNativeContextHandle());
    HWND hwnd = (HWND)(renderWindowNative->getNativeWindowHandle());

    QWGLNativeContext nativeContext(hglrc,hwnd);
    openGLContext->setNativeHandle(QVariant::fromValue(nativeContext));

    if(sharedOpenGLContext)
        openGLContext->setShareContext(sharedOpenGLContext);

    //Make sure native context is created
    bool contextCreated = openGLContext->create();
    assert(contextCreated);

    //Make sure we are sharing resources with the requested context
    if(sharedOpenGLContext)
    {
        bool sharing = QOpenGLContext::areSharing(openGLContext,sharedOpenGLContext);
        assert(sharing);
    }
}

void QTOpenGLRenderWindowNative::resize(unsigned int w, unsigned int h)
{
    renderWindowNative->resize(w,h);
}

void QTOpenGLRenderWindowNative::updateSpecs(OpenGLRendererAbstract::OpenGLRenderSpecs specs)
{
    renderWindowNative->updateSpecs(specs);
}

void QTOpenGLRenderWindowNative::renderFrame(GLuint texID, unsigned int width, unsigned int height)
{
    renderWindowNative->setFrame(texID,width,height);
    renderWindowNative->renderFrame();
}
