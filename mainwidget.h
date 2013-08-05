#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QMdiArea>
#include "mainwindow.h"

class QAction;
class QMenu;
class MainWindow;

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setView();
signals:
    
public slots:

    void openRecentFile();
    void updateRecentFileActions();
    void updateRecentFiles(QString &curFile);

    void createNewMainWindow();
    void openNewMainWindow();
    void closeAllWindow();
    void closeSubWindow();

    void showToolBar();
    void hideToolBar();

private:

    void setCurrentWindow();
    void initialRecentFilesActions();
    void createActions();
    void createToolBar();
    void connectSignalAndSlots(MainWindow *curWindow);

    QMdiArea *mdiArea;
    QStringList recentFiles;
    MainWindow *curWindow;
    QMdiSubWindow *curSubWindow;
    QToolBar *mainToolBar;

    QAction *createWindowAction;
    QAction *openWindowAction;
    QAction *closeAllWindowAction;

    QMenu *recentFilesMenu;
    enum{MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];

};

#endif // MAINWIDGET_H
