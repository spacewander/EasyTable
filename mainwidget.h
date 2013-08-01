#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QWorkspace>

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

signals:
    
public slots:
    void creatMainWindow();
    void closeMainWindow();
private:
    QWorkspace *workspace;
};

#endif // MAINWIDGET_H
