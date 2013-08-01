#include "mainwidget.h"
#include "mainwindow.h"

MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("EasyTable"));
    workspace = new QWorkspace;
    setCentralWidget(workspace);
    workspace->tile();
    creatMainWindow();

}

void MainWidget::creatMainWindow()
{
    MainWindow *window = new MainWindow;
    workspace->addWindow(window);
}

void MainWidget::closeMainWindow()
{

}
