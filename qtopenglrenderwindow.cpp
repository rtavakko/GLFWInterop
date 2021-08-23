#include "qtopenglrenderwindow.h"

QTOpenGLRenderWindow::QTOpenGLRenderWindow(QScreen *outputScreen,
                                           OpenGLRenderSpecs specs,
                                           const QSurfaceFormat &surfaceFormat,
                                           QOpenGLContext *sharedContext) :
    QWindow(outputScreen),
    QTOpenGLRenderer(specs),
    openGLFormat(surfaceFormat),
    openGLContext(nullptr),
    inputTextureID(0)
{
    //Create offscreen surface
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(openGLFormat);
    show();

    //Allocate memory for the context object and prepare to create it
    openGLContext = new QOpenGLContext(this);
    openGLContext->setFormat(openGLFormat);
    if(sharedContext)
        openGLContext->setShareContext(sharedContext);

    //Make sure the context is created & is sharing resources with the shared context
    bool contextCreated = openGLContext->create();
    assert(contextCreated);

    if(sharedContext)
    {
        bool sharing = QOpenGLContext::areSharing(openGLContext,sharedContext);
        assert(sharing);
    }

    openGLFormat = openGLContext->format();

    setTitle("QT Render Window (GUI Thread)");
    QWindow::resize(QSize(renderSpecs.frameType.width,renderSpecs.frameType.height));
}

QTOpenGLRenderWindow::~QTOpenGLRenderWindow()
{

}

bool QTOpenGLRenderWindow::event(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::UpdateRequest:
        renderFrame();
        return true;
        default:
        return QWindow::event(event);
    }
}

void QTOpenGLRenderWindow::exposeEvent(QExposeEvent *event)
{
    if(isExposed())
        renderFrame();
}

void QTOpenGLRenderWindow::resizeEvent(QResizeEvent *event)
{
    if(!isExposed())
        return;

    //TODO: FIX THIS; THE CONTEXT HAS TO BE ALREADY CREATED BY NOW SO NOT HAVING THIS SHOULDN'T CAUSE A CRASH
    if(!openGLContext)
        return;

    if(!makeContextCurrent())
        return;

    initialize();

    unsigned int w = event->size().width();
    unsigned int h = event->size().height();

    QWindow::resize(QSize(w,h));
    QTOpenGLRenderer::resize(w,h);

    swapSurfaceBuffers();
    doneContextCurrent();
}

const QSurfaceFormat &QTOpenGLRenderWindow::getOpenGLFormat()
{
    return openGLFormat;
}

QOpenGLContext *QTOpenGLRenderWindow::getOpenGLContext()
{
    return openGLContext;
}

void QTOpenGLRenderWindow::resize(unsigned int w, unsigned int h)
{
    if(!isExposed())
        return;

    if(!makeContextCurrent())
        return;

    initialize();

    QWindow::resize(QSize(w,h));
    QTOpenGLRenderer::resize(w,h);

    swapSurfaceBuffers();
    doneContextCurrent();
}

void QTOpenGLRenderWindow::updateSpecs(OpenGLRenderSpecs specs)
{
    if(!makeContextCurrent())
        return;

    initialize();

    QTOpenGLRenderer::updateSpecs(specs);

    swapSurfaceBuffers();
    doneContextCurrent();
}

void QTOpenGLRenderWindow::renderFrame()
{
    updateStartTime();

    if(!isExposed())
        return;

    if(!makeContextCurrent())
        return;

    initialize();

    //Render to default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(vaoID);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shader->bind();

    //Update shader uniform values
    updateUniforms();

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, inputTextureID);

    glViewport(0,0,width(),height());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    shader->release();

    glBindVertexArray(0);

    swapSurfaceBuffers();
    doneContextCurrent();

    updateEndTime();

    double actualFPS = 1000.0f/t_delta.count();

    emit renderedFrame(actualFPS);
}

void QTOpenGLRenderWindow::setFrame(GLuint texID, unsigned int width, unsigned int height)
{
    inputTextureID = texID;
    requestUpdate();
}

void QTOpenGLRenderWindow::renderLater()
{
    requestUpdate();
}

void QTOpenGLRenderWindow::swapSurfaceBuffers()
{
    openGLContext->swapBuffers(this);
}

bool QTOpenGLRenderWindow::makeContextCurrent()
{
    return openGLContext->makeCurrent(this);
}

void QTOpenGLRenderWindow::doneContextCurrent()
{
    openGLContext->doneCurrent();
}
