#ifndef QTOPENGLRENDERWINDOW_H
#define QTOPENGLRENDERWINDOW_H

#include <qtopenglrenderer.h>

#include <QWindow>
#include <QOpenGLContext>

#include <QResizeEvent>

class QTOpenGLRenderWindow : public QWindow, public QTOpenGLRenderer
{
    Q_OBJECT
public:
    QTOpenGLRenderWindow(QScreen* outputScreen,
                         OpenGLRenderSpecs specs,
                         const QSurfaceFormat& surfaceFormat,
                         QOpenGLContext* sharedContext);

    virtual ~QTOpenGLRenderWindow();

    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent* event) override;

    void resizeEvent(QResizeEvent *event) override;

    const QSurfaceFormat& getOpenGLFormat();
    QOpenGLContext* getOpenGLContext();

public slots:

    virtual void resize(unsigned int w, unsigned int h) override;
    virtual void updateSpecs(OpenGLRenderSpecs specs) override;

    virtual void renderFrame() override;

    void setFrame(GLuint texID, unsigned int width, unsigned int height);

    void renderLater();

signals:
    void renderedFrame(double actualFPS);

protected:
    void swapSurfaceBuffers();

    bool makeContextCurrent();
    void doneContextCurrent();

    QSurfaceFormat openGLFormat;
    QOpenGLContext* openGLContext;

    GLuint inputTextureID;
};

#endif // QTOPENGLRENDERWINDOW_H
