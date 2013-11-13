/**
*@class MainWidget
*the widget holds all windows
*it is the top level of this GUI
*/
/**
*@file
*the widget holds all windows
*it is the top level of this GUI
*/
#include "mainwidget.h"
#include "mainwindow.h"

#include <typeinfo>
#include <QAction>
#include <QMenu>
#include <QMdiSubWindow>
#include <QMouseEvent>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QToolBar>
#include <QMessageBox>
/**
*the constructor of MainWidget
*create a new MainWindow when the application is opened
*/
MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("EasyTable"));
    createActions();
    createToolBar();
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    setView();
    createNewMainWindow();
    this->showNormal();// a trick to let the initial performance look better
    ifCloseCancell = false;
}
/**
*set up the curWindow.Notice the curWindow may be nullptr
*when something unexpected happens.
*So remember to check the curWindow after calling this function.
*/
void MainWidget::setCurrentWindow()
{
    curWindow = new MainWindow(this);
    curSubWindow = mdiArea->addSubWindow(curWindow);
    connectSignalAndSlots(curWindow);
    curWindow->setFocus();
    this->showNormal();// a trick to let the initial performance look better
}
/**
*create actions in MainWidget
*/
void MainWidget::createActions()
{
    createWindowAction = new QAction(tr("&新建窗口"),this);
    connect(createWindowAction,SIGNAL(triggered()),
            this,SLOT(createNewMainWindow()));
    openWindowAction = new QAction(tr("&打开窗口"),this);
    connect(openWindowAction,SIGNAL(triggered()),
            this,SLOT(openNewMainWindow()));
    closeCurrentWindowAction = new QAction(tr("&关闭当前窗口"),this);
    connect(closeCurrentWindowAction,SIGNAL(triggered()),
            this,SLOT(closeSubWindow()));
    closeAllWindowAction = new QAction(tr("&关闭所有窗口"),this);
    connect(closeAllWindowAction,SIGNAL(triggered()),
            this,SLOT(closeAllWindow()));

    createRecentFileActions();
}
/**
*create actions associated with recent files
*by using initialRecentFilesActions()
*And this function generates the interface of recentFiles
*/
void MainWidget::createRecentFileActions()
{
    for(int i = 0;i<MaxRecentFiles;i++)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered()),
            this,SLOT(openRecentFile()));
    }
    initialRecentFilesActions();
}
/**
*create toolBar
*/
void MainWidget::createToolBar()
{
    mainToolBar = addToolBar(tr("文件"));
    mainToolBar->addAction(createWindowAction);
    mainToolBar->addAction(openWindowAction);
    mainToolBar->addAction(closeCurrentWindowAction);
    mainToolBar->addAction(closeAllWindowAction);
    mainToolBar->addSeparator();

    recentFilesMenu = new QMenu(tr("最近打开的文件"));
    recentFilesMenu->setCursor(QCursor(Qt::PointingHandCursor));
    for(int i = 0;i<MaxRecentFiles;i++)
    {
        recentFilesMenu->addAction(recentFileActions[i]);
    }
    mainToolBar->addAction(recentFilesMenu->menuAction());

    mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
    mainToolBar->setMovable(false);
    mainToolBar->setFloatable(false);
    mainToolBar->setMinimumHeight(30);
    mainToolBar->setAutoFillBackground(true);
    mainToolBar->setPalette(QPalette(Qt::lightGray));
    mainToolBar->setCursor(QCursor(Qt::PointingHandCursor));
}
/**
*connect signals and slots after a new MainWindow is created
*/
void MainWidget::connectSignalAndSlots(MainWindow *curWindow)
{
    connect(curWindow,SIGNAL(updateRecentFileActions()),
            this,SLOT(updateRecentFileActions()));
    connect(curWindow,SIGNAL(updateRecentFiles(QString&)),
            this,SLOT(updateRecentFiles(QString&)));
    //concurrent the recentFiles and recentFileActions
    //between MainWindow and MainWidget
    connect(curWindow,SIGNAL(exitApplication()),this,SLOT(closeAllWindow()));
    connect(curWindow,SIGNAL(closeSubWindow()),this,SLOT(closeSubWindow()));
    connect(curWindow,SIGNAL(showToolBar()),this,SLOT(showToolBar()));
    connect(curWindow,SIGNAL(hideToolBar()),this,SLOT(hideToolBar()));
    connect(curWindow,SIGNAL(closeCancelled()),this,SLOT(closeCancelled()));
}
/**
*create new MainWindow
*/
void MainWidget::createNewMainWindow()
{
    setCurrentWindow();
    hideToolBar();
    curWindow->showMaximized();
    curSubWindow->showMaximized();
}
/**
*open file in a new MainWindow
*/
void MainWidget::openNewMainWindow()
{
    setCurrentWindow();
    if(curWindow != nullptr)
        curWindow->open();
     hideToolBar();
     curSubWindow->showMaximized();
}
/**
*when close action is triggered,call this function to handle it.
*/
void MainWidget::closeEvent(QCloseEvent *event)
{
    closeAllWindow();
    if(!ifCloseCancell)
        event->accept();
    else if((mdiArea->subWindowList()).count() != 0)
        //if there is window existed
    {
        event->ignore();
        ifCloseCancell = false;
    }
}
/**
*close all windows
*/
void MainWidget::closeAllWindow()
{
    ifCloseCancell = false;
    QList<QMdiSubWindow*> widgetList = mdiArea->subWindowList();
    for(auto i = widgetList.begin();i != widgetList.end();i++)
    {
        (*i)->close();
        if(ifCloseCancell == true)
        {
            break;
        }
    }
    if(ifCloseCancell == false)
        close();
}
/**
*close current sub-window
*/
void MainWidget::closeSubWindow()
{
    curSubWindow = mdiArea->currentSubWindow();
    if(curSubWindow)
    {
        curSubWindow->close();
    }
//    if( mdiArea->subWindowList().count( ) <= 1)
//        //if there is only one window,close it despite whether curSubWindow exists or not
//    {
//        (*mdiArea->subWindowList().begin())->close();
//    }
    if(mdiArea->currentSubWindow())
        curSubWindow = mdiArea->currentSubWindow();
    if(mdiArea->subWindowList().size() == 0 || !mdiArea->currentSubWindow() )
        this->closeAllWindow();
}
/**
*update recentFile actions
*when a file is opened or created
*/
void MainWidget::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext())
    {
        if(!QFile::exists(i.next()))
            i.remove();
    }
    curWindow = new MainWindow(mdiArea->activeSubWindow());
    if(curWindow != nullptr)
    {
        for(int i = 0;i<MaxRecentFiles;i++)
        {
            if(i<recentFiles.count())
            {
                QString text = tr("&%1 %2").arg(i+1)
                    .arg(curWindow->
                         strippedName(recentFiles[i]));
                recentFileActions[i]->setText(text);
                recentFileActions[i]->setData(recentFiles[i]);
                recentFileActions[i]->setVisible(true);
            }
            else
            {
                recentFileActions[i]->setVisible(false);
            }//end if
        }//end for
    }//end if
}
/**
*this function aids updateRecentFileActions()
*/
void MainWidget::updateRecentFiles(QString &curFile)
{
    recentFiles.removeAll(curFile);
    recentFiles.prepend(curFile);
}
/**
*open a recent files
*/
void MainWidget::openRecentFile()
{
   setCurrentWindow();
   if(curWindow != nullptr)
   {
       QAction *action = qobject_cast<QAction *>(sender());
       if(action)
       {
           curWindow->loadFile(action->data().toString());
       }
   }
}
/**
*when mouse move across specific area
*show or hide the menubar of MainWidget
*/
void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y() >= 100)
        hideToolBar();
    else
        showToolBar();
}
/**
*when clicking left button of mouse in specific area
*show or hide the menubar of MainWidget
*/
void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->y() <= 30 && event->button() == Qt::LeftButton)
    {
        if(mainToolBar->isVisible())
            hideToolBar();
        else
            showToolBar();
    }
}
/**
*show toolBar and inform all windows
*/
void MainWidget::showToolBar()
{
    mainToolBar->show();
    QList<QMdiSubWindow*> widgetList = mdiArea->subWindowList();
    if(widgetList.isEmpty())
        return;
    for(auto i = widgetList.begin();i != widgetList.end();i++)
    {
        dynamic_cast<MainWindow*>((*i)->widget())->showMainToolBarAction->setEnabled(false);
        dynamic_cast<MainWindow*>((*i)->widget())->hideMainToolBarAction->setEnabled(true);
    }
}
/**
*hide toolBar and inform all windows
*/
void MainWidget::hideToolBar()
{
    mainToolBar->hide();
    QList<QMdiSubWindow*> widgetList = mdiArea->subWindowList();
    if(widgetList.isEmpty())
        return;
    for(auto i = widgetList.begin();i != widgetList.end();i++)
    {
        dynamic_cast<MainWindow*>((*i)->widget())->showMainToolBarAction->setEnabled(true);
        dynamic_cast<MainWindow*>((*i)->widget())->hideMainToolBarAction->setEnabled(false);
    }
}
/**
*initial recent files actions
*calling by createRecentFileActions()
*/
void MainWidget::initialRecentFilesActions()
{
    QSettings settings("BugMore Software Inc.","EasyTable");
    recentFiles = settings.value("recentFiles").toStringList();
    for(int i = 0;i<MaxRecentFiles;i++)
    {
        if(i<recentFiles.count())
        {
            QString text = tr("&%1 %2").arg(i+1)
                .arg(QFileInfo(recentFiles[i]).fileName());
            recentFileActions[i]->setText(text);
            recentFileActions[i]->setData(recentFiles[i]);
            recentFileActions[i]->setVisible(true);
        }
        else
        {
            recentFileActions[i]->setVisible(false);
        }//end if
    }//end for
}
/**
*set ifCloseCancell to true
*when close action is cancelled
*/
void MainWidget::closeCancelled()
{
    ifCloseCancell = true;
}
/**
*the user can set their preference view of MainWidget through this method
*/
void MainWidget::setView()
{
    mdiArea->tileSubWindows();
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::South);
    mdiArea->setTabShape(QTabWidget::Triangular);
    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);
}
