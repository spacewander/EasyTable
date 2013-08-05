#include "mainwidget.h"
#include "mainwindow.h"

#include <QAction>
#include <QMenu>
#include <QMdiSubWindow>
#include <QMouseEvent>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QToolBar>

MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("EasyTable"));
    createActions();
    createToolBar();
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    setView();
    setCurrentWindow();
}

void MainWidget::setCurrentWindow()
//set up the curWindow.Notice the curWindow may be nullptr
//when something unexpected happens.
//So remember to check the curWindow after calling this function.
{
    curWindow = new MainWindow(this);
    curSubWindow = mdiArea->addSubWindow(curWindow);
    connectSignalAndSlots(curWindow);
    curWindow->setFocus();
    mdiArea->activatePreviousSubWindow();
}

void MainWidget::createActions()
{
    createWindowAction = new QAction(tr("&新建窗口"),this);
    connect(createWindowAction,SIGNAL(triggered()),
            this,SLOT(createNewMainWindow()));
    openWindowAction = new QAction(tr("&打开窗口"),this);
    connect(openWindowAction,SIGNAL(triggered()),
            this,SLOT(openNewMainWindow()));
    closeAllWindowAction = new QAction(tr("&关闭所有窗口"),this);
    connect(closeAllWindowAction,SIGNAL(triggered()),
            this,SLOT(closeAllWindow()));

    for(int i = 0;i<MaxRecentFiles;i++)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered()),
            this,SLOT(openRecentFile()));
    }
    initialRecentFilesActions();
}

void MainWidget::createToolBar()
{
    mainToolBar = addToolBar(tr("文件"));
    mainToolBar->addAction(createWindowAction);
    mainToolBar->addAction(openWindowAction);
    mainToolBar->addAction(closeAllWindowAction);
    mainToolBar->addSeparator();

    recentFilesMenu = new QMenu(tr("最近打开的文件"));
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
}

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
}

void MainWidget::createNewMainWindow()
{
    setCurrentWindow();
    mainToolBar->hide();
}

void MainWidget::openNewMainWindow()
{
    setCurrentWindow();
    if(curWindow != nullptr)
        curWindow->open();
     mainToolBar->hide();
}

void MainWidget::closeAllWindow()
{
    QList<QMdiSubWindow*> widgetList = mdiArea->subWindowList();
    for(auto i = widgetList.begin();i != widgetList.end();i++)
    {
        (*i)->close();
    }
    close();
}

void MainWidget::closeSubWindow()
{
    curSubWindow = mdiArea->currentSubWindow();
    curSubWindow->close();
}

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
                recentFileActions[i]->setData(recentFiles);
                recentFileActions[i]->setVisible(true);
            }
            else
            {
                recentFileActions[i]->setVisible(false);
            }//end if
        }//end for
    }//end if
}

void MainWidget::updateRecentFiles(QString &curFile)
{
    recentFiles.removeAll(curFile);
    recentFiles.prepend(curFile);
}

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

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y() >= 100)
        mainToolBar->hide();
    else
        mainToolBar->show();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->y() <= 30 && event->button() == Qt::LeftButton)
        mainToolBar->show();
}

void MainWidget::showToolBar()
{
    mainToolBar->show();
}

void MainWidget::hideToolBar()
{
    mainToolBar->hide();
}

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
            recentFileActions[i]->setData(recentFiles);
            recentFileActions[i]->setVisible(true);
        }
        else
        {
            recentFileActions[i]->setVisible(false);
        }//end if
    }//end for
}

void MainWidget::setView()
//the user can set their preference view through this method
{
    mdiArea->tileSubWindows();
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::South);
    mdiArea->setTabShape(QTabWidget::Triangular);
    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);
}
