#ifndef OPENGLRENDERERABSTRACT_H
#define OPENGLRENDERERABSTRACT_H

//#include <QOpenGLExtraFunctions>

#include <GLES3/gl3.h>

#include <chrono>
#include <ctime>

class OpenGLRendererAbstract
{
public:
    //Defines an OpenGL texture
    typedef struct OpenGLTextureSpecs
    {
        unsigned int width;
        unsigned int height;
        unsigned int channels;

        GLenum target;

        GLint internalFormat;
        GLenum format;

        GLenum dataType;
    }
    OpenGLTextureSpecs;

    //Defines an OpenGL renderer
    typedef struct OpenGLRenderSpecs
    {
        OpenGLTextureSpecs frameType;

        double frameRate;
    }
    OpenGLRenderSpecs;

    OpenGLRendererAbstract(OpenGLRenderSpecs specs);

    virtual ~OpenGLRendererAbstract();

    virtual GLuint getTextureID() const;

    virtual OpenGLRenderSpecs getSpecs() const;

    //These are the main functions we will use
    virtual void initialize();
    virtual void resize(unsigned int w, unsigned int h);
    virtual void updateSpecs(OpenGLRenderSpecs specs);

    virtual void setFrameRate(float fps);

    virtual void renderFrame() = 0;

protected:
    virtual void initializeFBO() = 0;
    virtual void initializeShaderProgram() = 0;
    virtual void initializeVertexBuffers() = 0;
    virtual void initializeUniforms() = 0;

    virtual void resizeFBO() = 0;

    virtual void updateUniforms() = 0;

    virtual void updateStartTime();
    virtual void updateEndTime();

    virtual void swapSurfaceBuffers() = 0;

    virtual bool makeContextCurrent() = 0;
    virtual void doneContextCurrent() = 0;

    //Variables
    bool initialized;

    OpenGLRenderSpecs renderSpecs;

    const GLenum GL_outputColorAttachment = GL_COLOR_ATTACHMENT0;

    GLint vertexAttributeLocation;
    GLint texCoordAttributeLocation;

    GLint textureUniformLocation;

    GLuint vaoID;
    GLuint vboID;
    GLuint fboID;

    GLint textureUnit;
    GLuint outputTextureID;

    //Used for timing
    std::chrono::time_point<std::chrono::high_resolution_clock> t_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> t_end;

    std::chrono::duration<double,std::milli> t_delta;
};

#endif // OPENGLRENDERERABSTRACT_H
