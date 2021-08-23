#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    renderer(nullptr),
    renderWindow(nullptr),
    renderWindowNative(nullptr),
    renderThread(nullptr)
{
    ui->setupUi(this);

    bool initialized = initialize();
    assert(initialized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::initialize()
{
    renderThread = new QThread();

    OpenGLRendererAbstract::OpenGLRenderSpecs renderSpecs =
    OpenGLRendererAbstract::OpenGLRenderSpecs{
    OpenGLRendererAbstract::OpenGLTextureSpecs{
            640,
            360,
            4,
            GL_TEXTURE_2D,
            GL_RGBA8,
            GL_RGBA,
            GL_UNSIGNED_BYTE
    },
            60.0f};

    renderer = new QTOpenGLRenderSurface(nullptr,
                                         nullptr,
                                         renderSpecs,
                                         QSurfaceFormat::defaultFormat(),
                                         nullptr);
    /*
    renderer = new TriangleRendererWindow(nullptr,
                                          renderSpecs,
                                          QSurfaceFormat::defaultFormat(),
                                          nullptr);
    */

    renderWindow = new QTOpenGLRenderWindow(nullptr,
                                            renderSpecs,
                                            renderer->getOpenGLFormat(),
                                            renderer->getOpenGLContext());

    renderWindowNative = new QTOpenGLRenderWindowNative(renderer);

    renderer->moveToThread(renderThread);
    renderWindowNative->moveToThread(renderThread);

    QObject::connect(renderer,&QTOpenGLRenderSurface::frameReady,renderWindow,&QTOpenGLRenderWindow::setFrame);
    QObject::connect(renderer,&QTOpenGLRenderSurface::frameReady,renderWindowNative,&QTOpenGLRenderWindowNative::renderFrame);

    //QObject::connect(renderer,&TriangleRendererWindow::frameReady,renderWindow,&QTOpenGLRenderWindow::setFrame);
    //QObject::connect(renderer,&TriangleRendererWindow::frameReady,renderWindowNative,&QTOpenGLRenderWindowNative::renderFrame);

    QObject::connect(renderThread,&QThread::started,renderer,&QTOpenGLRenderSurface::start);

    //QObject::connect(renderThread,&QThread::started,renderer,&TriangleRendererWindow::start);

    renderThread->start();

    return true;
}
