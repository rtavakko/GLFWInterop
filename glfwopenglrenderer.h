#ifndef GLFWOPENGLRENDERER_H
#define GLFWOPENGLRENDERER_H

#include <GL/glew.h>

#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <openglrendererabstract.h> //gl.h has to be included after glew.h

#include <QDebug>
#include <QFile>

class GLFWOpenGLRenderer : public OpenGLRendererAbstract
{
public:
    GLFWOpenGLRenderer(OpenGLRenderSpecs specs);

    virtual ~GLFWOpenGLRenderer();

    //These are the main functions we will use
    virtual void initialize() override;

protected:
    virtual GLuint loadShaders(std::string vertShader, std::string fragShader);

    virtual bool initializeGLEW();

    virtual void initializeFBO() override;
    virtual void initializeShaderProgram() override;
    virtual void initializeVertexBuffers() override;
    virtual void initializeUniforms() override;

    virtual void resizeFBO() override;

    virtual void updateUniforms() override;

    //GLEW OpenGL resources
    GLuint programID;
};

#endif // GLFWOPENGLRENDERER_H
