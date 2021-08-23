#ifndef QTOPENGLRENDERER_H
#define QTOPENGLRENDERER_H

#include <openglrendererabstract.h>

//
#include <QOpenGLExtraFunctions>
//

#include <QOpenGLShaderProgram>
#include <QTimer>

#include <chrono>
#include <ctime>

class QTOpenGLRenderer : public QOpenGLExtraFunctions, public OpenGLRendererAbstract
{
public:
    QTOpenGLRenderer(OpenGLRenderSpecs specs);

    virtual ~QTOpenGLRenderer();

    //These are the main functions we will use
    virtual void initialize() override;

protected:
    virtual void initializeFBO() override;
    virtual void initializeShaderProgram() override;
    virtual void initializeVertexBuffers() override;
    virtual void initializeUniforms() override;

    virtual void resizeFBO() override;

    virtual void updateUniforms() override;

    //Variables
    QOpenGLShaderProgram* shader;
};

#endif // QTOPENGLRENDERER_H
