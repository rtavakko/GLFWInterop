#include "glfwopenglrenderwindow.h"

GLFWOpenGLRenderWindow::GLFWOpenGLRenderWindow(OpenGLRendererAbstract::OpenGLRenderSpecs specs,
                                               const QSurfaceFormat &surfaceFormat,
                                               WId shareWindow) :
    GLFWOpenGLRenderer(specs),
    window(nullptr),
    sharedWindowHandle(shareWindow),
    sharedWindow(nullptr),
    inputTextureID(0),
    testInit(false),
    testTextureID(0),
    openGLFormat(surfaceFormat)
{ 
    //Initialize GLFW first to create a window and OpenGL context
    bool GLFWInitialized = initializeGLFW();
    assert(GLFWInitialized);
}

void GLFWOpenGLRenderWindow::renderFrame()
{
    if(!makeContextCurrent())
         return;

     initialize();

     //TEST
     //if(!testInit)
         //initializeTestTexture();
     //

     //Render to default FBO
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     glBindVertexArray(vaoID);

     glClearColor(0.0f,0.0f,0.0f,1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

     glUseProgram(programID);

     //Update shader uniform values
     updateUniforms();

     glActiveTexture(GL_TEXTURE0 + textureUnit);
     glBindTexture(GL_TEXTURE_2D, inputTextureID);

     glViewport(0,0,renderSpecs.frameType.width,renderSpecs.frameType.height);

     glDrawArrays(GL_TRIANGLES, 0, 6);

     qDebug()<<glGetError();            //This outputs 0 but nothing renders on the screen

     glBindTexture(GL_TEXTURE_2D, 0);
     glActiveTexture(GL_TEXTURE0);

     glUseProgram(0);

     glBindVertexArray(0);

     swapSurfaceBuffers();
     doneContextCurrent();

     glfwPollEvents();

     updateEndTime();
}

void GLFWOpenGLRenderWindow::setFrame(GLuint texID, unsigned int width, unsigned int height)
{
    inputTextureID = texID;
}

WId GLFWOpenGLRenderWindow::getNativeWindowHandle() const
{
    #if defined GLFW_EXPOSE_NATIVE_WIN32
    return (WId)(glfwGetWin32Window(window));
    #elif defined GLFW_EXPOSE_NATIVE_COCOA
    return (WId)(glfwGetCocoaWindow(window));
    #elif defined GLFW_EXPOSE_NATIVE_WAYLAND
    return (WId)(glfwGetWaylandWindow(window));
    #else
    return (WId)(NULL);
    #endif
}

void *GLFWOpenGLRenderWindow::getNativeContextHandle() const
{
    #if defined GLFW_EXPOSE_NATIVE_WIN32
    return (void*)(glfwGetWGLContext(window));
    #elif defined GLFW_EXPOSE_NATIVE_COCOA
    return (void*)(glfwGetCocoaWindow(window));
    #elif defined GLFW_EXPOSE_NATIVE_WAYLAND
    return (void*)(glfwGetWaylandWindow(window));
    #else
    return (void*)(NULL);
    #endif
}

bool GLFWOpenGLRenderWindow::initializeGLFW()
{
    if(!glfwInit())
        return false;

    //Create main window
    glfwSwapInterval(openGLFormat.swapInterval());

    glfwWindowHint(GLFW_SAMPLES, openGLFormat.samples());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLFormat.majorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLFormat.minorVersion());
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                                                                                                //To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, (openGLFormat.profile()==QSurfaceFormat::CoreProfile)?GLFW_OPENGL_CORE_PROFILE:GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_DEPTH_BITS, openGLFormat.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, openGLFormat.stencilBufferSize());

    glfwWindowHint(GLFW_RED_BITS, openGLFormat.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS, openGLFormat.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS, openGLFormat.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS, openGLFormat.alphaBufferSize());

    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    glfwWindowHint(GLFW_STEREO, openGLFormat.stereo()?GLFW_TRUE:GLFW_FALSE);

    glfwWindowHint(GLFW_SRGB_CAPABLE, (openGLFormat.colorSpace()==QSurfaceFormat::sRGBColorSpace)?GLFW_TRUE:GLFW_FALSE);

    glfwWindowHint(GLFW_CLIENT_API, (openGLFormat.renderableType()==QSurfaceFormat::OpenGL?GLFW_OPENGL_API:((openGLFormat.redBufferSize()==QSurfaceFormat::OpenGLES)?GLFW_OPENGL_ES_API:GLFW_NO_API)));

    glfwWindowHint(GLFW_DOUBLEBUFFER, (openGLFormat.swapBehavior()==QSurfaceFormat::DoubleBuffer)?GLFW_TRUE:GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, (openGLFormat.options()==QSurfaceFormat::ResetNotification)?GLFW_LOSE_CONTEXT_ON_RESET:GLFW_NO_ROBUSTNESS);

    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);

    //Create shared window first
    #if defined GLFW_EXPOSE_NATIVE_WIN32
    sharedWindow = glfwAttachWin32Window((HWND)(sharedWindowHandle),NULL);
    #elif defined GLFW_EXPOSE_NATIVE_COCOA
    #elif defined GLFW_EXPOSE_NATIVE_WAYLAND
    #else
    #endif

    if( sharedWindow == NULL )
    {
        glfwTerminate();
        return false;
    }

    //Open a window and create its OpenGL context
    window = glfwCreateWindow( renderSpecs.frameType.width, renderSpecs.frameType.height, "Native Render Window (Non-GUI Thread)", glfwGetWindowMonitor(sharedWindow), sharedWindow);
    if( window == NULL )
    {
        glfwTerminate();
        return false;
    }

    return true;
}

void GLFWOpenGLRenderWindow::swapSurfaceBuffers()
{
    glfwSwapBuffers(window);
}

bool GLFWOpenGLRenderWindow::makeContextCurrent()
{
    if(!window)
        return false;

    if(window == glfwGetCurrentContext())
        return true;

    glfwMakeContextCurrent(window);
    return (window == glfwGetCurrentContext());
}

void GLFWOpenGLRenderWindow::doneContextCurrent()
{
    glfwMakeContextCurrent(NULL);
}

void GLFWOpenGLRenderWindow::initializeTestTexture()
{
    if(testInit)
        return;

    //Load watermark image
    testImage.load(QString(":/GLSL/test.jpg"));
    testImage = testImage.convertToFormat(QImage::Format_RGBA8888);

    assert(!testImage.isNull() && (testImage.format() == QImage::Format_RGBA8888));;

    glGenTextures(1, &testTextureID);

    glBindTexture(GL_TEXTURE_2D, testTextureID);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, renderSpecs.frameType.internalFormat, testImage.width(), testImage.height(), 0, renderSpecs.frameType.format, renderSpecs.frameType.dataType, (const GLvoid*)(testImage.bits()));
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    testInit = true;
}
