#ifndef QTOPENGLRENDERSURFACE_H
#define QTOPENGLRENDERSURFACE_H

#include <qtopenglrenderer.h>

#include <QOffscreenSurface>
#include <QOpenGLContext>

class QTOpenGLRenderSurface : public QOffscreenSurface, public QTOpenGLRenderer
{
    Q_OBJECT
public:    
    QTOpenGLRenderSurface(QScreen* outputScreen,
                          QObject* parent,
                          OpenGLRenderSpecs specs,
                          const QSurfaceFormat& surfaceFormat,
                          QOpenGLContext* sharedContext);

    virtual ~QTOpenGLRenderSurface();

    const QSurfaceFormat& getOpenGLFormat();
    QOpenGLContext* getOpenGLContext();

public slots:    

    virtual void setFrameRate(float fps) override;

    virtual void start();
    virtual void stop();

    virtual void renderFrame() override;

signals:
    void frameReady(GLuint texID, unsigned int width, unsigned int height);
    void renderedFrame(double actualFPS);

protected:

    virtual void initializeFBO() override;
    virtual void initializeShaderProgram() override;
    virtual void initializeVertexBuffers() override;
    virtual void initializeUniforms() override;

    virtual void updateUniforms() override;

    virtual void initializeTimer();

    virtual void swapSurfaceBuffers() override;

    virtual bool makeContextCurrent() override;
    virtual void doneContextCurrent() override;

    //Sync timer
    QTimer* syncTimer;

    QSurfaceFormat openGLFormat;
    QOpenGLContext* openGLContext;

    //For rendering a debug triangle
    GLuint depthrenderbuffer;

    GLint trianglePositionAttributeLocation;
    GLint triangleColorAttributeLocation;
    GLint triangleMatrixUniformLocation;

    float triangleAngle;
};

#endif // QTOPENGLRENDERSURFACE_H
