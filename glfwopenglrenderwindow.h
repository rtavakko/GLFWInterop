#ifndef GLFWOPENGLRENDERWINDOW_H
#define GLFWOPENGLRENDERWINDOW_H

#include <glfwopenglrenderer.h>

#include <QSurfaceFormat>
#include <QImage>

class GLFWOpenGLRenderWindow : public GLFWOpenGLRenderer
{
public:
    GLFWOpenGLRenderWindow(OpenGLRenderSpecs specs,
                           const QSurfaceFormat& surfaceFormat,
                           WId shareWindow);

    virtual void renderFrame() override;

    void setFrame(GLuint texID, unsigned int width, unsigned int height);

    //Windows native resource access
    WId getNativeWindowHandle() const;
    void* getNativeContextHandle() const;

protected:
    virtual bool initializeGLFW();

    virtual void swapSurfaceBuffers() override;

    virtual bool makeContextCurrent() override;
    virtual void doneContextCurrent() override;

    //GLEW OpenGL resources
    GLFWwindow* window;

    WId sharedWindowHandle;
    GLFWwindow* sharedWindow;

    GLuint inputTextureID;

    QSurfaceFormat openGLFormat;

    //TEST
    virtual void initializeTestTexture();

    bool testInit;

    QImage testImage;
    GLuint testTextureID;
    //
};

#endif // GLFWOPENGLRENDERWINDOW_H
