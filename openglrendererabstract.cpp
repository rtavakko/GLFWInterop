#include "openglrendererabstract.h"

OpenGLRendererAbstract::OpenGLRendererAbstract(OpenGLRendererAbstract::OpenGLRenderSpecs specs) :
    initialized(false),
    renderSpecs(specs),
    vertexAttributeLocation(0),
    texCoordAttributeLocation(0),
    textureUniformLocation(0),
    vaoID(0),
    vboID(0),
    fboID(0),
    textureUnit(0),
    outputTextureID(0)
{

}

OpenGLRendererAbstract::~OpenGLRendererAbstract()
{

}

GLuint OpenGLRendererAbstract::getTextureID() const
{
    return outputTextureID;
}

OpenGLRendererAbstract::OpenGLRenderSpecs OpenGLRendererAbstract::getSpecs() const
{
    return renderSpecs;
}

void OpenGLRendererAbstract::initialize()
{
    if(initialized)
        return;

    initializeShaderProgram();
    initializeVertexBuffers();
    initializeFBO();
    initializeUniforms();

    initialized = true;
}

void OpenGLRendererAbstract::resize(unsigned int w, unsigned int h)
{
    if(w == 0 || h == 0)
        return;

    if(renderSpecs.frameType.width != w || renderSpecs.frameType.height != h)
    {
        renderSpecs.frameType.width = w;
        renderSpecs.frameType.height = h;

        resizeFBO();
    }
}

void OpenGLRendererAbstract::updateSpecs(OpenGLRendererAbstract::OpenGLRenderSpecs specs)
{
    if(renderSpecs.frameType.width == 0 || renderSpecs.frameType.height == 0 || renderSpecs.frameType.channels == 0)
        return;

    if(
            renderSpecs.frameType.width != specs.frameType.width ||
            renderSpecs.frameType.height != specs.frameType.height ||
            renderSpecs.frameType.channels != specs.frameType.channels ||
            renderSpecs.frameRate != specs.frameRate ||
            renderSpecs.frameType.internalFormat != specs.frameType.internalFormat ||
            renderSpecs.frameType.format != specs.frameType.format ||
            renderSpecs.frameType.dataType != specs.frameType.dataType)
    {
        renderSpecs = specs;

        resizeFBO();
    }
}

void OpenGLRendererAbstract::setFrameRate(float fps)
{
    if(fps <= 0.0f)
        return;

    renderSpecs.frameRate = fps;
}

void OpenGLRendererAbstract::updateStartTime()
{
    t_start = std::chrono::high_resolution_clock::now();
}

void OpenGLRendererAbstract::updateEndTime()
{
    t_end = std::chrono::high_resolution_clock::now();
    t_delta = t_end - t_start;
}
