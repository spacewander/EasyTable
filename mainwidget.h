/**
*@file
*the widget holds all windows
*it is the top level of this GUI
*/
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
    void closeEvent(QCloseEvent *event);
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
    void closeCancelled();

    void showToolBar();
    void hideToolBar();

private:
    void setCurrentWindow();
    void initialRecentFilesActions();
    void createActions();
    void createRecentFileActions();
    void createToolBar();
    void connectSignalAndSlots(MainWindow *curWindow);

    QMdiArea *mdiArea;
    QStringList recentFiles;
    MainWindow *curWindow;
    QMdiSubWindow *curSubWindow;
    QToolBar *mainToolBar;

    QAction *createWindowAction;
    QAction *openWindowAction;
    QAction *closeCurrentWindowAction;
    QAction *closeAllWindowAction;

    QMenu *recentFilesMenu;
	/// the max number of recent files displayed
    enum{MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];

    bool ifCloseCancell;
};

#endif // MAINWIDGET_H
