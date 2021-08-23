#include "glfwopenglrenderer.h"

GLFWOpenGLRenderer::GLFWOpenGLRenderer(OpenGLRendererAbstract::OpenGLRenderSpecs specs) :
    OpenGLRendererAbstract(specs),
    programID(0)
{

}

GLFWOpenGLRenderer::~GLFWOpenGLRenderer()
{

}

void GLFWOpenGLRenderer::initialize()
{
    if(initialized)
        return;

    bool GLEWInitialized = initializeGLEW();
    assert(GLEWInitialized);

    initializeShaderProgram();
    initializeVertexBuffers();
    initializeFBO();
    initializeUniforms();

    initialized = true;
}

GLuint GLFWOpenGLRenderer::loadShaders(std::string vertShader, std::string fragShader)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    char const * VertexSourcePointer = vertShader.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    char const * FragmentSourcePointer = fragShader.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        qDebug()<<QString("Program (%1v,%2) failed to compile:").arg(vertShader.c_str()).arg(fragShader.c_str());
        qDebug()<<ProgramErrorMessage.data();
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

bool GLFWOpenGLRenderer::initializeGLEW()
{
    glewExperimental = true;                                        // Needed in core profile
    if (glewInit() != GLEW_OK)                                      // Initialize GLEW
        return false;    

    return true;
}

void GLFWOpenGLRenderer::initializeFBO()
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

void GLFWOpenGLRenderer::initializeShaderProgram()
{
    //Create main OpenGL program; add vertex and fragment shaders, link and bind
    QFile vertFile(QString(":/GLSL/passVertex.glsl"));
    QFile fragFile(QString(":/GLSL/passFragment.glsl"));

    if (!vertFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if (!fragFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    programID = loadShaders(vertFile.readAll().toStdString(),
                            fragFile.readAll().toStdString());

    glUseProgram(programID);

    //Get locations of vertex shader attributes
    vertexAttributeLocation = glGetAttribLocation(programID, "vertex");
    texCoordAttributeLocation = glGetAttribLocation(programID, "texCoord");

    textureUniformLocation = glGetUniformLocation(programID, "texture");

    glGetUniformiv(programID, textureUniformLocation, &textureUnit);   //Set the value of the texture unit (GL_TEXTUREX) so it can be used in glActiveTexture

    glUseProgram(0);
}

void GLFWOpenGLRenderer::initializeVertexBuffers()
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

void GLFWOpenGLRenderer::initializeUniforms()
{
    //Resize output texture
    glBindTexture(GL_TEXTURE_2D, outputTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, renderSpecs.frameType.internalFormat, renderSpecs.frameType.width, renderSpecs.frameType.height, 0, renderSpecs.frameType.format, renderSpecs.frameType.dataType, (const GLvoid*)(nullptr));

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFWOpenGLRenderer::resizeFBO()
{
    //Resize output texture
    glBindTexture(GL_TEXTURE_2D, outputTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, renderSpecs.frameType.internalFormat, renderSpecs.frameType.width, renderSpecs.frameType.height, 0, renderSpecs.frameType.format, renderSpecs.frameType.dataType, (const GLvoid*)(nullptr));

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFWOpenGLRenderer::updateUniforms()
{

}
