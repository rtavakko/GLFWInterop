#include "qtopenglrenderer.h"

QTOpenGLRenderer::QTOpenGLRenderer(OpenGLRenderSpecs specs) :
    OpenGLRendererAbstract(specs),
    shader(nullptr)
{
}

QTOpenGLRenderer::~QTOpenGLRenderer()
{
    if(shader)
        delete shader;

    shader = nullptr;
}

void QTOpenGLRenderer::initialize()
{
    if(initialized)
        return;

    initializeOpenGLFunctions();

    initializeShaderProgram();
    initializeVertexBuffers();
    initializeFBO();
    initializeUniforms();

    initialized = true;
}

void QTOpenGLRenderer::initializeFBO()
{
    //Generate output FBO and texture
    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    glGenTextures(1, &outputTextureID);

    glBindTexture(GL_TEXTURE_2D, outputTextureID);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, renderSpecs.frameType.internalFormat, renderSpecs.frameType.width, renderSpecs.frameType.height, 0, renderSpecs.frameType.format, renderSpecs.frameType.dataType, (const GLvoid*)(nullptr));
    glGenerateMipmap(GL_TEXTURE_2D);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_outputColorAttachment, GL_TEXTURE_2D, outputTextureID, 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void QTOpenGLRenderer::initializeShaderProgram()
{
    //Create main OpenGL program; add vertex and fragment shaders, link and bind
    shader = new QOpenGLShaderProgram();
    shader -> addShaderFromSourceFile(QOpenGLShader::Vertex, QString(":/GLSL/passVertex.glsl"));
    shader -> addShaderFromSourceFile(QOpenGLShader::Fragment, QString(":/GLSL/passFragment.glsl"));

    shader -> link();
    shader -> bind();

    //Get locations of vertex shader attributes
    vertexAttributeLocation = glGetAttribLocation(shader -> programId(), "vertex");
    texCoordAttributeLocation = glGetAttribLocation(shader -> programId(), "texCoord");

    textureUniformLocation = glGetUniformLocation(shader -> programId(), "texture");

    glGetUniformiv(shader->programId(), textureUniformLocation, &textureUnit);   //Set the value of the texture unit (GL_TEXTUREX) so it can be used in glActiveTexture

    shader->release();
}

void QTOpenGLRenderer::initializeVertexBuffers()
{
    //Vertex and texture positions of image quad
    static const GLfloat vertexData[6][4] = {{-1.0f,-1.0f,0.0f,0.0f},{1.0f,-1.0f,1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f},
                                             {-1.0f,-1.0f,0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f},{-1.0f,1.0f,0.0f,1.0f}};
    //Generate and bind VAO
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    //Generate and bind VBO
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexAttributeLocation);
    glEnableVertexAttribArray(texCoordAttributeLocation);

    glVertexAttribPointer(vertexAttributeLocation, 2, GL_FLOAT, GL_TRUE, 4*sizeof(GLfloat), (const void*)(0));
    glVertexAttribPointer(texCoordAttributeLocation, 2, GL_FLOAT, GL_TRUE, 4*sizeof(GLfloat), (const void*)(2*sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void QTOpenGLRenderer::initializeUniforms()
{
}

void QTOpenGLRenderer::resizeFBO()
{
    //Resize output texture
    glBindTexture(GL_TEXTURE_2D, outputTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, renderSpecs.frameType.internalFormat, renderSpecs.frameType.width, renderSpecs.frameType.height, 0, renderSpecs.frameType.format, renderSpecs.frameType.dataType, (const GLvoid*)(nullptr));

    glBindTexture(GL_TEXTURE_2D, 0);
}

void QTOpenGLRenderer::updateUniforms()
{
}
