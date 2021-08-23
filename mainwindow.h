#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QThread>

#include <qtopenglrenderwindownative.h>

#include <qtopenglrendersurface.h>
#include <qtopenglrenderwindow.h>

#include <trianglerendererwindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void setRenderFPS(double fps);

private:
    bool initialize();

    Ui::MainWindow *ui;

    QTOpenGLRenderSurface* renderer;
    //TriangleRendererWindow* renderer;

    QTOpenGLRenderWindow* renderWindow;

    QTOpenGLRenderWindowNative* renderWindowNative;

    QThread* renderThread;
};

#endif // MAINWINDOW_H
