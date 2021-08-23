#ifndef TRIANGLERENDERERWINDOW_H
#define TRIANGLERENDERERWINDOW_H

#include <qtopenglrenderwindow.h>

class TriangleRendererWindow : public QTOpenGLRenderWindow
{
    Q_OBJECT
public:
    TriangleRendererWindow(QScreen* outputScreen,
                           OpenGLRenderSpecs specs,
                           const QSurfaceFormat& surfaceFormat,
                           QOpenGLContext* sharedContext);

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

#endif // TRIANGLERENDERERWINDOW_H
