//recommand to use <QtGui>
#include <QCheckBox>
#include <QColorDialog>
#include <QCloseEvent>
#include <QDialog>
#include <QEvent>

#include <QFileDialog>
#include <QFontDialog>
#include <QIcon>
#include <QLabel>
#include <QList>
#include <QLineEdit>

#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>

#include <QSettings>
#include <QStatusBar>
#include <QTableWidgetSelectionRange>
#include <QTimer>
#include <QTextDocument>
#include <QToolBar>
#include <QWidget>

#include "easytable.h"
#include "finddialog.h"
#include "gotocelldialog.h"
#include "mainwindow.h"
#include "sortdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    sheet = new EasyTable;
    setCentralWidget(sheet);
    timer = nullptr;//initialize the timer
    setAutoSave(false);
    createActions();
    createMenus();
    createContextMenu();
    createStatusBar();
    createToolBars();
    readSettings();
    findDialog = nullptr;
    tipToolBar = nullptr;
    tipMap = sheet->tipMap;
    setCurrentFile(tr("未命名"));
}

void MainWindow::createActions()
{
    createFileActions();
    createRecentFileActions();
    createEditActions();
    createFormulaActions();
    createToolActions();
    createOptionActions();
    createOtherActions();
}

void MainWindow::createFileActions()
//new,open,save,saveAs,print,close,exit,minimize,maximize,normalize
{
    newAction = new QAction(tr("&新建"),this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("新建文件"));
    connect(newAction,SIGNAL(triggered()),this,SLOT(newFile()));
    openAction = new QAction(tr("&打开"),this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("打开文件"));
    connect(openAction,SIGNAL(triggered()),this,SLOT(open()));
    saveAction = new QAction(tr("&保存"),this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("保存文件"));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
    saveAsAction = new QAction(tr("另存为"),this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("另存为"));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));
    printAction = new QAction(tr("打印"),this);
    printAction->setShortcut(QKeySequence::Print);
    printAction->setStatusTip(tr("打印"));
    connect(printAction,SIGNAL(triggered()),this,SLOT(print()));

    closeAction = new QAction(tr("&关闭"),this);
    closeAction->setStatusTip(tr("关闭"));
    connect(closeAction,SIGNAL(triggered()),this,SLOT(closeWindow()));
    exitAction = new QAction(tr("退出"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("退出"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(closeAllWindow()));

    minimizeAction = new QAction(tr("最小化"),this);
    minimizeAction->setShortcut(QKeySequence::ZoomOut);//use ctrl -
    minimizeAction->setStatusTip(tr("最小化"));
    connect(minimizeAction,SIGNAL(triggered()),this,SLOT(showMinimized()));
    maximizeAction = new QAction(tr("最大化"),this);
    maximizeAction->setShortcut(QKeySequence::ZoomIn);//use ctrl +
    maximizeAction->setStatusTip(tr("最大化"));
    connect(maximizeAction,SIGNAL(triggered()),this,SLOT(showMaximized()));
    normalizeAction = new QAction(tr("向下还原"),this);
    normalizeAction->setStatusTip(tr("还原"));
    connect(normalizeAction,SIGNAL(triggered()),this,SLOT(showNormal()));
}

void MainWindow::createEditActions()
//cut,copy,paste,delete,insert,remove,hide,select
{
    cutAction = new QAction(tr("剪切"),this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("剪切"));
    connect(cutAction,SIGNAL(triggered()),sheet,SLOT(cut()));
    copyAction = new QAction(tr("复制"),this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("复制"));
    connect(copyAction,SIGNAL(triggered()),sheet,SLOT(copy()));
    pasteAction = new QAction(tr("粘贴"),this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("粘贴"));
    connect(pasteAction,SIGNAL(triggered()),sheet,SLOT(paste()));
    deleteAction = new QAction(tr("清除"),this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("清除"));
    connect(deleteAction,SIGNAL(triggered()),sheet,SLOT(del()));

    insertRowAction = new QAction(tr("插入行"),this);
    insertRowAction->setStatusTip(tr("插入行"));
    connect(insertRowAction,SIGNAL(triggered()),sheet,SLOT(rowInsert()));
    insertColumnAction = new QAction(tr("插入列"),this);
    insertColumnAction->setStatusTip(tr("插入列"));
    connect(insertColumnAction,SIGNAL(triggered()),sheet,SLOT(columnInsert()));
    removeRowAction = new QAction(tr("删除行"),this);
    removeRowAction->setStatusTip(tr("删除行"));
    connect(removeRowAction,SIGNAL(triggered()),sheet,SLOT(rowRemove()));
    removeColumnAction = new QAction(tr("删除列"),this);
    removeColumnAction->setStatusTip(tr("删除列"));
    connect(removeColumnAction,SIGNAL(triggered()),sheet,SLOT(columnRemove()));

    hideRowsAction = new QAction(tr("隐藏选中行"),this);
    hideRowsAction->setStatusTip(tr("隐藏选中行"));
    connect(hideRowsAction,SIGNAL(triggered()),sheet,SLOT(rowHide()));
    hideColumnsAction = new QAction(tr("隐藏选中列"),this);
    hideColumnsAction->setStatusTip(tr("隐藏选中列"));
    connect(hideColumnsAction,SIGNAL(triggered()),sheet,SLOT(columnHide()));
    showHiddenRangesAction = new QAction(tr("显示所有隐藏区域"),this);
    connect(showHiddenRangesAction,SIGNAL(triggered()),sheet,SLOT(showHiddenRanges()));

    selectRowAction = new QAction(tr("选中行"),this);
    selectRowAction->setStatusTip(tr("选择行"));
    connect(selectRowAction,SIGNAL(triggered()),sheet,SLOT(selectCurrentRow()));
    selectColumnAction = new QAction(tr("选中列"),this);
    selectColumnAction->setStatusTip(tr("选择列"));
    connect(selectColumnAction,SIGNAL(triggered()),sheet,SLOT(selectCurrentColumn()));
    selectAllAction = new QAction(tr("选中全部"),this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("选择所有单元格"));
    connect(selectAllAction,SIGNAL(triggered()),sheet,SLOT(selectAll()));
}

void MainWindow::createOptionActions()
//showGrid,defaultAlignment,autoResize,autoSave,autoTip,show/hideMainToolBar
{
    showGridAction = new QAction(tr("&显示边线"),this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(sheet->showGrid());
    showGridAction->setStatusTip(tr("显示或隐藏单元格的边线"));
    connect(showGridAction,SIGNAL(toggled(bool)),
        sheet,SLOT(setShowGrid(bool)));

    defaultAlignmentAction = new QAction(tr("&使用默认对齐"),this);
    defaultAlignmentAction->setCheckable(true);
    defaultAlignmentAction->setChecked(sheet->getDefaultAlignment());
    defaultAlignmentAction->setStatusTip(tr("默认对齐"));
    connect(defaultAlignmentAction,SIGNAL(toggled(bool)),
        sheet,SLOT(setDefaultAlignment(bool)));

    autoResizeAction = new QAction(tr("&使用自动调整"),this);
    autoResizeAction->setCheckable(true);
    autoResizeAction->setChecked(sheet->getAutoResize());
    autoResizeAction->setStatusTip(tr("自动调整"));
    connect(autoResizeAction,SIGNAL(toggled(bool)),
        sheet,SLOT(setAutoResize(bool)));

    autoSaveAction = new QAction(tr("&使用定时保存"),this);
    autoSaveAction->setCheckable(true);
    autoSaveAction->setChecked(autoSave);
    autoSaveAction->setStatusTip(tr("定时保存"));
    connect(autoSaveAction,SIGNAL(toggled(bool)),
        this,SLOT(setAutoSave(bool)));

    autoTipAction = new QAction(tr("&使用智能提示"),this);
    autoTipAction->setCheckable(true);
    autoTipAction->setChecked(sheet->getAutoTip());
    autoTipAction->setStatusTip(tr("智能提示"));
    autoTipAction->setShortcut(tr("Alt+t"));
    connect(autoTipAction,SIGNAL(toggled(bool)),
        this,SLOT(createTipToolBar(bool)));

    showMainToolBarAction = new QAction(tr("显示窗口菜单栏"),this);
    showMainToolBarAction->setShortcut(tr("Alt+s"));
    connect(showMainToolBarAction,SIGNAL(triggered()),this,SIGNAL(showToolBar()));
    hideMainToolBarAction = new QAction(tr("隐藏窗口菜单栏"),this);
    hideMainToolBarAction->setShortcut(tr("Alt+h"));
    connect(hideMainToolBarAction,SIGNAL(triggered()),this,SIGNAL(hideToolBar()));
}

void MainWindow::createToolActions()
//find,goToCell,function,recalculate,sort,groupBy
{
    findAction = new QAction(tr("查找"),this);
    findAction->setStatusTip(tr("查找"));
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction,SIGNAL(triggered()),this,SLOT(find()));
    goToCellAction = new QAction(tr("前往"),this);
    goToCellAction->setStatusTip(tr("前往"));
    goToCellAction->setShortcut(tr("Ctrl+T"));
    connect(goToCellAction,SIGNAL(triggered()),this,SLOT(goToCell()));
    functionAction = new QAction(tr("函数"),this);
    functionAction->setStatusTip(tr("应用函数"));
    connect(functionAction,SIGNAL(triggered()),sheet,SLOT(useFunction()));

    recalculateAction = new QAction(tr("重新计算"),this);
    recalculateAction->setStatusTip(tr("重新计算表格中的公式"));
    connect(recalculateAction,SIGNAL(triggered()),sheet,SLOT(recalculate()));
    autoRecalcAction  =  new QAction(tr("自动重新计算"),this);
    autoRecalcAction->setStatusTip(tr("设置是否自动重新计算表格中的公式"));
    connect(autoRecalcAction,SIGNAL(triggered(bool)),sheet,SLOT(setAutoRecalculate(bool)));

    sortAction = new QAction(tr("排序"),this);
    sortAction->setStatusTip(tr("按条件排序"));
    sortAction->setShortcut(tr("Shift+O"));
    connect(sortAction,SIGNAL(triggered()),this,SLOT(sort()));

    groupByAction = new QAction(tr("分类汇总"),this);
    groupByAction->setStatusTip(tr("分类汇总"));
    connect(groupByAction,SIGNAL(triggered()),this,SLOT(createGroupByToolBar()));
    hideGroupByAction = new QAction(tr("取消分类汇总"),this);
    connect(hideGroupByAction,SIGNAL(triggered()),this,SLOT(cancellGroupBy()));
}

void MainWindow::createFormulaActions()
//font,color,alignment
{
    fontAction = new QAction(tr("字体"),this);
    connect(fontAction,SIGNAL(triggered()),this,SLOT(setFont()));
    textColorAction = new QAction(tr("字体颜色"),this);
    connect(textColorAction,SIGNAL(triggered()),this,SLOT(setTextColor()));

    textColorIconAction = new QAction(this);
    textColorIcon = setIconColor(textColorIcon,Qt::black);
    textColorIconAction->setIcon(textColorIcon);
    connect(textColorIconAction,SIGNAL(triggered()),this,SLOT(setTextColor()));

    backgroundColorAction = new QAction(tr("单元格颜色"),this);
    connect(backgroundColorAction,SIGNAL(triggered()),this,SLOT(setBackgroundColor()));

    backgroundColorIconAction = new QAction(this);
    backgroundColorIcon = setIconColor(backgroundColorIcon,Qt::white);
    backgroundColorIconAction->setIcon(backgroundColorIcon);
    connect(backgroundColorIconAction,SIGNAL(triggered()),this,SLOT(setBackgroundColor()));

    leftAlignmentAction = new QAction(tr("左对齐"),this);
    connect(leftAlignmentAction,SIGNAL(triggered()),this,SLOT(setLeftAlignment()));
    centerAlignmentAction = new QAction(tr("居中"),this);
    connect(centerAlignmentAction,SIGNAL(triggered()),this,SLOT(setCenterAlignment()));
    rightAlignmentAction = new QAction(tr("右对齐"),this);
    connect(rightAlignmentAction,SIGNAL(triggered()),this,SLOT(setRightAlignment()));
    topAlignmentAction = new QAction(tr("基于顶线对齐"),this);
    connect(topAlignmentAction,SIGNAL(triggered()),this,SLOT(setTopAlignment()));
    bottomAlignmentAction = new QAction(tr("基于底线对齐"),this);
    connect(bottomAlignmentAction,SIGNAL(triggered()),this,SLOT(setBottomAlignment()));
}

void MainWindow::createOtherActions()
//about,gridStyle
{
    aboutAction = new QAction(tr("&关于"),this);
    aboutAction->setStatusTip(tr("关于我们"));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

    noneGridStyleAction = new QAction(tr("无边线"),this);
    connect(noneGridStyleAction,SIGNAL(triggered()),this,SLOT(hideGrid()));
    solidGridStyleAction = new QAction(tr("实线"),this);
    connect(solidGridStyleAction,SIGNAL(triggered()),this,SLOT(setSolidGrid()));
    connect(solidGridStyleAction,SIGNAL(triggered()),this,SLOT(showGrid()));
    dashGridStyleAction = new QAction(tr("虚线"),this);
    connect(dashGridStyleAction,SIGNAL(triggered()),this,SLOT(setDashGrid()));
    connect(dashGridStyleAction,SIGNAL(triggered()),this,SLOT(showGrid()));
    dotGridStyleAction = new QAction(tr("点线"),this);
    connect(dotGridStyleAction,SIGNAL(triggered()),this,SLOT(setDotGrid()));
    connect(dotGridStyleAction,SIGNAL(triggered()),this,SLOT(showGrid()));
}

void MainWindow::createRecentFileActions()
{
    for(int i = 0;i<MaxRecentFiles;i++)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered()),
            this,SLOT(openRecentFile()));
    }

}

void MainWindow::createSubMenus()
{
    removeSubMenu = new QMenu(tr("删除"));
    removeSubMenu->addAction(removeRowAction);
    removeSubMenu->addAction(removeColumnAction);

    formatSubMenu = new QMenu(tr("单元格格式"));
    formatSubMenu->addAction(fontAction);
    formatSubMenu->addAction(textColorAction);
    formatSubMenu->addAction(backgroundColorAction);

    insertSubMenu = new QMenu(tr("插入"));
    insertSubMenu->addAction(insertRowAction);
    insertSubMenu->addAction(insertColumnAction);

    alignmentSubMenu = new QMenu(tr("对齐"));
    alignmentSubMenu->addAction(leftAlignmentAction);
    alignmentSubMenu->addAction(centerAlignmentAction);
    alignmentSubMenu->addAction(rightAlignmentAction);
    alignmentSubMenu->addAction(topAlignmentAction);
    alignmentSubMenu->addAction(bottomAlignmentAction);

    chooseSubMenu = new QMenu(tr("选择"));
    chooseSubMenu->addAction(selectRowAction);
    chooseSubMenu->addAction(selectColumnAction);
    chooseSubMenu->addAction(selectAllAction);

    hideSubMenu = new QMenu(tr("隐藏"));
    hideSubMenu->addAction(hideRowsAction);
    hideSubMenu->addAction(hideColumnsAction);

    gridStyleSubMenu = new QMenu(tr("边线风格"));
    gridStyleSubMenu->setCursor(QCursor(Qt::PointingHandCursor));
    gridStyleSubMenu->addAction(noneGridStyleAction);
    gridStyleSubMenu->addAction(solidGridStyleAction);
    gridStyleSubMenu->addAction(dashGridStyleAction);
    gridStyleSubMenu->addAction(dotGridStyleAction);

    recentFilesSubMenu = new QMenu(tr("最近打开的文件"));
    recentFilesSubMenu->setCursor(QCursor(Qt::PointingHandCursor));
    for(int i = 0;i<MaxRecentFiles;i++)
    {
        recentFilesSubMenu->addAction(recentFileActions[i]);
    }
}

void MainWindow::createMenus()
{
    menuBar()->adjustSize();
    menuBar()->setMinimumHeight(20);
    createSubMenus();

    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->setCursor(QCursor(Qt::PointingHandCursor));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(printAction);

    separatorAction = fileMenu->addSeparator();

    fileMenu->addMenu(recentFilesSubMenu);
    fileMenu->addSeparator();

    fileMenu->addAction(minimizeAction);
    fileMenu->addAction(maximizeAction);
    fileMenu->addAction(normalizeAction);
    fileMenu->addSeparator();

    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("编辑"));
    editMenu->setCursor(QCursor(Qt::PointingHandCursor));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);

    editMenu->addSeparator();
    editMenu->addAction(chooseSubMenu->menuAction());
    editMenu->addAction(insertSubMenu->menuAction());
    editMenu->addAction(removeSubMenu->menuAction());
    editMenu->addAction(gridStyleSubMenu->menuAction());
    editMenu->addAction(hideSubMenu->menuAction());
    editMenu->addSeparator();
    editMenu->addAction(showHiddenRangesAction);

    toolsMenu = menuBar()->addMenu(tr("工具"));
    toolsMenu->setCursor(QCursor(Qt::PointingHandCursor));
    toolsMenu->addAction(sortAction);
    toolsMenu->addAction(findAction);
    toolsMenu->addAction(goToCellAction);
    toolsMenu->addAction(groupByAction);
    toolsMenu->addAction(hideGroupByAction);
    hideGroupByAction->setVisible(false);

    optionsMenu = menuBar()->addMenu(tr("选项"));
    optionsMenu->setCursor(QCursor(Qt::PointingHandCursor));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(defaultAlignmentAction);
    optionsMenu->addAction(autoResizeAction);
    optionsMenu->addAction(autoSaveAction);
    optionsMenu->addAction(autoTipAction);
    optionsMenu->addAction(showMainToolBarAction);
    optionsMenu->addAction(hideMainToolBarAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->setCursor(QCursor(Qt::PointingHandCursor));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createContextMenu()
{
    sheet->addAction(cutAction);
    sheet->addAction(copyAction);
    sheet->addAction(pasteAction);
    sheet->addAction(deleteAction);

    sheet->addAction(functionAction);
    sheet->addAction(formatSubMenu->menuAction());
    sheet->addAction(alignmentSubMenu->menuAction());
    sheet->addAction(insertSubMenu->menuAction());
    sheet->addAction(removeSubMenu->menuAction());
    sheet->addAction(chooseSubMenu->menuAction());
    sheet->addAction(hideSubMenu->menuAction());
    sheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->setCursor(QCursor(Qt::PointingHandCursor));
    fileToolBar->setMinimumHeight(30);
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addSeparator();

    editToolBar = addToolBar(tr("编辑"));
    editToolBar->setCursor(QCursor(Qt::PointingHandCursor));
    editToolBar->setMinimumHeight(30);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
    editToolBar->addAction(sortAction);
    editToolBar->addSeparator();

    formatToolBar = addToolBar(tr("单元格格式"));
    formatToolBar->setCursor(QCursor(Qt::PointingHandCursor));
    formatToolBar->setMinimumHeight(30);
    formatToolBar->addAction(fontAction);
    formatToolBar->addAction(textColorAction);
    formatToolBar->addAction(textColorIconAction);
    formatToolBar->addSeparator();
    formatToolBar->addAction(backgroundColorAction);
    formatToolBar->addAction(backgroundColorIconAction);
    formatToolBar->addSeparator();

    gridStyleToolBar = addToolBar(tr("边线风格"));
    gridStyleToolBar->addAction(gridStyleSubMenu->menuAction());
    gridStyleToolBar->setMinimumHeight(30);

    alignmentToolBar = addToolBar(tr("对齐"));
    alignmentToolBar->setCursor(QCursor(Qt::PointingHandCursor));
    insertToolBarBreak(alignmentToolBar);
    alignmentToolBar->setMinimumHeight(30);
    alignmentToolBar->addAction(leftAlignmentAction);
    alignmentToolBar->addAction(centerAlignmentAction);
    alignmentToolBar->addAction(rightAlignmentAction);
    alignmentToolBar->addSeparator();
    alignmentToolBar->addAction(topAlignmentAction);
    alignmentToolBar->addAction(bottomAlignmentAction);
    alignmentToolBar->addSeparator();
}

void MainWindow::createGroupByToolBar()
{
    groupByAction->setEnabled(false);
    groupByToolBar = addToolBar(tr("分类汇总"));
    createGroupByToolBarView();
    int ColumnCount = sheet->getColumnCount();
    for(int i = 0;i<ColumnCount;i++)
    {
        QSet<QString> context;
        sheet->getColumnContext(i,context,maxRow);
        if(context.count() == 0)
            continue;
        QMenu *subMenu = new QMenu(tr("%1列").arg(QString('A' + i)));
        for(auto it = context.begin(); it != context.end();it++)
        {
            QAction *action = new QAction(this);
            action->setText(*it);
            QString str = (*it) +'_' + QString(i+'0');
            //the formula of action->Data is like "hello_2"
            action->setData(str);
            connect(action,SIGNAL(triggered()),this,SLOT(hideSpecificRow()));
            subMenu->addAction(action);
        }
        QAction *menuAction = new QAction(this);
        menuAction = subMenu->menuAction();
        menuAction->setToolTip(tr("显示符合条件的行和空行"));
        groupByToolBar->addAction(menuAction);
        groupByToolBar->addSeparator();
    }
    hideGroupByAction->setVisible(true);
}

void MainWindow::createGroupByToolBarView()
{
    insertToolBarBreak(groupByToolBar);
    groupByToolBar->setMinimumHeight(30);
    groupByToolBar->setAllowedAreas(Qt::TopToolBarArea);
    groupByToolBar->setMovable(false);
    groupByToolBar->setFloatable(false);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel("Z999");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel,1);

    connect(sheet,SIGNAL(currentCellChanged(int,int,int,int)),
        this,SLOT(updateStatusBar()));
    connect(sheet,SIGNAL(modified()),this,SLOT(sheetModified()));
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(sheet->currentLocation());
    formulaLabel->setText(sheet->currentFormula());
}

void MainWindow::sheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::setAutoSave(bool ok)
{
    autoSave = ok;
    if(autoSave)
        saveAfterTimeInterval();
}

void MainWindow::saveAfterTimeInterval()
{
    if(autoSave)
    {
        if(timer == nullptr)
            timer = new QTimer(this);
        if(isWindowModified())
        {
            statusBar()->showMessage(tr("文件已自动保存"),10000);
            //keep the message for 10 seconds
            save();
        }
        connect(timer,SIGNAL(timeout()),this,SLOT(saveAfterTimeInterval()));
        timer->start(600 * 1000);//save after 10 minutes
    }
    else
    {
        delete timer;
        timer = nullptr;
    }
}

void MainWindow::newFile()
{
    if(okToContinue())
    {
        sheet->clear();
        setCurrentFile("");
        return;
    }
}

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int r = QMessageBox::warning(this,tr("EasyTable"),
            tr("%1 已经改变。\n"
            "是否保存?").arg(strippedName(curFile)),
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        switch(r)
        {
        case QMessageBox::Yes:
            return save();
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::No:
            setWindowModified(false);
            break;
        default:
            return false;
        }
    }
    return true;
}

void MainWindow::open()
{
    if(okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("打开表格" ),
            ".",
            tr("easytable files (*.et)\n"));//set the openable type
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(!sheet->readFile(fileName))
    {
        QString str = tr("%1 取消加载").arg(fileName);
        statusBar()->showMessage(str,2000);//display for 2 sec
        return false;
    }
    setCurrentFile(fileName);
    QString str = tr("加载文件 %1").arg(fileName);
    statusBar()->showMessage(str,2000);
    return true;
}

bool MainWindow::save()
{
    if(curFile.isEmpty() || curFile == tr("未命名"))
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(!sheet->writeFile(fileName))
    {
        statusBar()->showMessage(tr("取消保存"),2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("文件保存"),2000);
    setWindowModified(false);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,tr("保存表格"),
        ".",
        tr("easytable files (*.et)\n"
           "纯文本文件 (*.txt)\n"));//set the save-able type
    if(fileName.isEmpty())
    {
        return false;
    }
    return saveFile(fileName);
}

void MainWindow::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec())
    {
        QTextDocument *text = sheet->getContextForPrint();
        text->print(&printer);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
        emit closeCancelled();
    }
}

void MainWindow::closeWindow()
{
    close();
    if(!isWindowModified())
        emit closeSubWindow();
    //avoid the SubWindow checks whether the window has been modified.
}

void MainWindow::mousePressEvent(QMouseEvent *event)
//use click left button of mouse to show the toolBar in MainWidget
{
    if(event->y() <= 60)
    {
        if(event->button() == Qt::LeftButton)
        {
            emit showToolBar();
            statusBar()->showMessage(tr("显示窗口菜单栏"));
        }
        else
        {
            emit hideToolBar();
            statusBar()->showMessage(tr("隐藏窗口菜单栏"));
        }
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);
    QString shownName = tr("未命名");
    if((!curFile.isEmpty()))
    {
        if(curFile != tr("未命名"))
        {
            shownName = strippedName(curFile);
            recentFiles.removeAll(curFile);
            recentFiles.prepend(curFile);
            refreshRecentFileActions();
            emit updateRecentFiles(curFile);
            emit updateRecentFileActions();
        }
        else
        {
            refreshRecentFileActions();
            emit updateRecentFileActions();
        }
    }
    setWindowTitle(tr("%1[*]-%2").arg(shownName).arg(tr("sheet")));
}

QString MainWindow::strippedName(const QString &fullFileName)
//strip the fileName from path
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::refreshRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext())
    {
        if(!QFile::exists(i.next()))
            i.remove();
    }
    for(int j = 0;j<MaxRecentFiles;j++)
    {
        if(j<recentFiles.count())
        {
            QString text = tr("&%1 %2").arg(j+1)
                .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
        {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::openRecentFile()
{
    if(okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        //transform QObject* to QAction*
        if(action)
            loadFile(action->data().toString());
    }
}

void MainWindow::closeAllWindow()
{
    emit exitApplication();
}

void MainWindow::find()
{
    if(!findDialog)
    {
        findDialog = new FindDialog(this);
        connect(findDialog,SIGNAL(findNext(const QString&,
            Qt::CaseSensitivity)),
            sheet,SLOT(findNext(const QString&,
            Qt::CaseSensitivity)));

        connect(findDialog,SIGNAL(findPrevious(const QString&,
            Qt::CaseSensitivity)),
            sheet,SLOT(findPrevious(const QString&,
            Qt::CaseSensitivity)));

        connect(findDialog,SIGNAL(findInAll(const QString&,
            Qt::CaseSensitivity)),
            sheet,SLOT(findInAll(const QString&,
            Qt::CaseSensitivity)));

        connect(findDialog,SIGNAL(findFromHere(const QString&,
            Qt::CaseSensitivity)),
            sheet,SLOT(findFromHere(const QString&,
            Qt::CaseSensitivity)));

        connect(findDialog,SIGNAL(replaceSelectedCell(const QString&)),
                sheet,SLOT(replaceSelectedCell(const QString&)));
    }
    findDialog->setWindowOpacity(1.0);
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::goToCell()
{
    GotoCellDialog *dialog = new GotoCellDialog(this);
    if(dialog->exec())
    {
        int row = dialog->getRow();
        int column = dialog->getColumn();
        row = row < sheet->getRowCount() ?
                    row : sheet->getRowCount();
        column = column < sheet->getColumnCount() ?
                    column : sheet->getColumnCount();
        sheet->setCurrentCell(row,column);
    }
    delete dialog;
}

void MainWindow::sort()
{
    SortDialog dialog(this);
    QTableWidgetSelectionRange range = sheet->selectedRange();
    if (range.leftColumn() != range.rightColumn())
    {
        dialog.setColumnRange('A'+range.leftColumn(),
        'A'+range.rightColumn());
    }
    else
    {
    dialog.setColumnRange('A',
        'A'+sheet->getColumnCount());
    }
    if(dialog.exec())
    {
        EasyTableCompare compare;
        dialog.setSortKeyandAscending(compare);
        bool defaultChoose = true;
        int choice = QMessageBox::question(this,tr("排序"),
                                           tr("发现您只选取了一部分区域。\n"
                                               "我们将自动扩展排序的范围。\n"
            "是否同意?"),
            QMessageBox::Yes|QMessageBox::No);
        switch(choice)
        {
        case QMessageBox::Yes:
            sheet->sort(compare,defaultChoose);
            break;
        case QMessageBox::No:
            defaultChoose = false;
            sheet->sort(compare,defaultChoose);
            break;
        default:
            break;
        }
    }
}

void MainWindow::showGrid()
{
    sheet->setShowGrid(true);
    showGridAction->setChecked(true);
}

void MainWindow::hideGrid()
{
    sheet->setShowGrid(false);
    showGridAction->setChecked(false);
}

void MainWindow::setDashGrid()
{
    Qt::PenStyle style = Qt::DashLine;
    sheet->setGrid(style);
}

void MainWindow::setDotGrid()
{
    Qt::PenStyle style = Qt::DotLine;
    sheet->setGrid(style);
}

void MainWindow::setSolidGrid()
{
    Qt::PenStyle style = Qt::SolidLine;
    sheet->setGrid(style);
}

void MainWindow::setFont()
{
    bool ok;
    QFont curFont;
    if(!sheet->currentItem())
        curFont = sheet->currentItem()->font();
    QFont font = QFontDialog::getFont(&ok,curFont);
    //set default font of QFontDialog to the current font,
    //if the font of cell has been changed
    if(ok)
    {
       sheet->setFont(font);
    }
}

void MainWindow::setLeftAlignment()
{
    int alignmentCode = Qt::AlignLeft | Qt::AlignVCenter;
    //if not indicated, the alignment type should be center in vertical
    sheet->setAlignment(alignmentCode);
}

void MainWindow::setCenterAlignment()
{
    int alignmentCode = Qt::AlignCenter;
    sheet->setAlignment(alignmentCode);
}

void MainWindow::setRightAlignment()
{
    int alignmentCode = Qt::AlignRight | Qt::AlignVCenter;
    sheet->setAlignment(alignmentCode);
}

void MainWindow::setTopAlignment()
{
    int alignmentCode = Qt::AlignTop | Qt::AlignHCenter;
    //if not indicated, the alignment type should be center in horizon
    sheet->setAlignment(alignmentCode);
}

void MainWindow::setBottomAlignment()
{
    int alignmentCode = Qt::AlignBottom | Qt::AlignHCenter;
    sheet->setAlignment(alignmentCode);
}

QIcon& MainWindow::setIconColor(QIcon &icon,QColor color)
{
    QSize suitableSize(70,50);//The size is suitable for toolbar
    QPixmap pixmapColor(suitableSize);
    pixmapColor.fill(color);
    icon.addPixmap(pixmapColor);
    return icon;
}

void MainWindow::setTextColor()
{
    QColor curColor;
    if(!sheet->currentItem())
        curColor = sheet->currentItem()->textColor();
    QColor textColor;
    if(curColor.isValid())
        textColor = QColorDialog::getColor(curColor);
    //set default color of QColorDialog to the current color,
    //if the color of cell has been changed
    else
        textColor = QColorDialog::getColor(Qt::black);
    if(textColor.isValid())
    {
        textColorIcon = setIconColor(textColorIcon,textColor);
        textColorIconAction->setIcon(textColorIcon);
        formatToolBar->updateGeometry();
        setWindowModified(false);
        sheet->setTextColor(textColor);
    }
}

void MainWindow::setBackgroundColor()
{
    QColor curColor;
    if(!sheet->currentItem())
        curColor = sheet->currentItem()->backgroundColor();
    QColor backgroundColor;
    if(curColor.isValid())
        backgroundColor = QColorDialog::getColor(curColor);
    //set default color of QColorDialog to the current color,
    //if the color of cell has been changed
    else
        backgroundColor = QColorDialog::getColor(Qt::white);
    if(backgroundColor.isValid())
    {
        backgroundColorIcon = setIconColor(backgroundColorIcon,backgroundColor);
        backgroundColorIconAction->setIcon(backgroundColorIcon);
        formatToolBar->updateGeometry();
        sheet->setBackgroundColor(backgroundColor);
        setWindowModified(false);
    }
}

void MainWindow::hideSpecificRow()
{
    QAction *action = qobject_cast<QAction *>(sender());
    //transform QObject* to QAction*
    if(action)
    {
        sheet->showHiddenRanges();
        QString str = action->data().toString();
//action->data is like "hello_0",the part before _ is the context of model
//and the part after is the number of column
        QStringList strList = str.split('_');
        int column = strList[1].toInt();
        int range = 0;
        for(auto it = maxRow.begin();it != maxRow.end();it++)
        {
            if((*it)>range)
                range = *it;
        }
        sheet->hideRowUnlike(column,strList[0],range);
    }
}

void MainWindow::cancellGroupBy()
{
    sheet->showHiddenRanges();
    groupByToolBar->hide();
    groupByAction->setEnabled(true);
    hideGroupByAction->setVisible(false);
    maxRow.clear();
}

void MainWindow::about()
{
    QMessageBox::about(this,tr("About EasyTable"),
        tr("<h1>EasyTable 1.0</h1>"
           "<br/>"
        "<em>Copyleft &copy; BugMore Software Inc.</em>"));
}

void MainWindow::writeSettings()
{
    QSettings settings("BugMore Software Inc.","EasyTable");
    settings.setValue("geometry",saveGeometry());
    settings.setValue("recentFiles",recentFiles);
    settings.setValue("showGrid",showGridAction->isChecked());
    settings.setValue("defaultAlignment",defaultAlignmentAction->isChecked());
    settings.setValue("autoRecalc",autoRecalcAction->isChecked());
    settings.setValue("autoResize",autoResizeAction->isChecked());
    settings.setValue("autoSave",autoSaveAction->isChecked());
}

void MainWindow::readSettings()
{
    QSettings settings("BugMore Software Inc.","EasyTable");
    restoreGeometry(settings.value("geometry").toByteArray());
    recentFiles = settings.value("recentFiles").toStringList();
    refreshRecentFileActions();
    emit updateRecentFileActions();
    bool showGrid = settings.value("showGrid",true).toBool();
    showGridAction->setChecked(showGrid);
    bool defaultAlignment = settings.value("defaultAlignment",true).toBool();
    defaultAlignmentAction->setChecked(defaultAlignment);
    bool autoRecalc = settings.value("autoRecalc",true).toBool();
    autoRecalcAction->setChecked(autoRecalc);
    bool autoResize = settings.value("autoResize",true).toBool();
    autoResizeAction->setChecked(autoResize);
    bool autoSave = settings.value("autoSave",false).toBool();
    autoSaveAction->setChecked(autoSave);
}

void MainWindow::createTipToolBar(bool ok)
{
    sheet->setAutoTip(ok);
    if(ok)
    {
        connect(sheet,SIGNAL(modified()),this,SLOT(refreshTipTool()));
        connect(sheet,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(refreshTipTool()));
        if(sheet->tipMap.isEmpty())
            sheet->initialTipMap();
        if(tipToolBar != nullptr)
        {
            tipToolBar->show();
            return;
        }
        tipToolBar = addToolBar(tr("补全"));
        createTipToolBarView();
        createTipToolBarActions();
        tipToolBar->show();
    }
    else
    {
        tipToolBar->hide();
        disconnect(sheet,SIGNAL(modified()),this,SLOT(refreshTipTool()));
        disconnect(sheet,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(refreshTipTool()));
    }
}

void MainWindow::createTipToolBarView()
{
    tipToolBar->setToolTip(tr("使用Alt + 数字 进行补全"));
    insertToolBarBreak(tipToolBar);
    tipToolBar->setMinimumHeight(30);
    tipToolBar->setAllowedAreas(Qt::TopToolBarArea);
    tipToolBar->setMovable(false);
    tipToolBar->setFloatable(false);
    QLabel *label = new QLabel(tr("使用Alt + 数字 进行补全"));
    tipToolBar->addWidget(label);
    tipToolBar->addSeparator();
    tipToolBar->setPalette(QPalette(Qt::green));
}

void MainWindow::refreshTipTool()
{
    //if tipToolBar is not existed in the UI
    if(tipToolBar == nullptr)
        createTipToolBar(true);
    else if(!tipToolBar->isVisible())
        tipToolBar->show();
    //else
    else
    {
        QString str = sheet->getCurrentText();
        int column = sheet->currentColumn();
        if(str.isEmpty())
        {
            createTipToolBarActions(column);
            tipToolBar->setPalette(QPalette(Qt::magenta));
        }
        else
        {
            createTipToolBarActions(column,str);
            tipToolBar->setPalette(QPalette(Qt::cyan));
        }//end for cell is not empty
    }//end for the tipToolBar is visible
}

void MainWindow::destroyTipToolBarActions()
//delete all the actions in tipToolBar
{
    QList<QAction*> listAct = tipToolBar->actions();
    if(listAct.isEmpty())
        return;
    //if there are actions in tipToolBar,clear it
    for(auto it = listAct.begin() + 2;it != listAct.end();++it)
    {
        delete *it;
        *it = nullptr;
    }
    /*
     *  Note that we need not to delete the first  two action
     *  Because the first action is an action to show the tip label
     *  and the second one is a separator action
    */
}

/*
  *  The three functions below create the actions in tipToolBar,so before you
  *  use them,make sure the tipToolBar is created.
  *  And I also limit the items(actions) in tipToolBar less than 10.
  *  The result why I limit the items in tipToolBar should be no more than 10
  *  is that too much items squeezed in a narrow place does not look good.
  *  And it is not useful to create too much items since you will be unable
  *  find your target in a look.
  */

void MainWindow::createTipToolBarActions()
//show the 10 items in the beginning of tipMap
{
	if(sheet->getTipDirty() == true)
		tipMap = sheet->tipMap;
    int count = tipMap.count();
    QMap<QString,int>::iterator it = tipMap.end() - 1;
    destroyTipToolBarActions();
    for(int i = 0;i < 10 && i<count;i++)
    {
        QAction *action = new QAction(this);
        action->setShortcut(tr("Alt+%1").arg(i));
        //so i should be smaller than 10
        action->setText(tr("%1: ").arg(i)+it.key());
        action->setData(it.key());
        connect(action,SIGNAL(triggered()),this,SLOT(finishCell()));
        tipToolBar->addAction(action);
        tipToolBar->addSeparator();
        --it;
    }
}

void MainWindow::createTipToolBarActions(int column)
//show the 10 items which is in the particular column
{
     if(sheet->getTipDirty() == true)
		tipMap = sheet->tipMap;
     destroyTipToolBarActions();
     QList<QString> strList = tipMap.keys(column);
     QList<QString>::iterator it = strList.begin();
     int count = strList.count();
     if(count == 0)
     {
        //if the column is an empty column
         createTipToolBarActions();
         return;
     }
     for(int i = 0;i < 10 && i<count;i++)
     {
         QAction *action = new QAction(this);
         action->setShortcut(tr("Alt+%1").arg(i));
         action->setText(tr("%1: ").arg(i) + (*it));
         action->setData(*it);
         connect(action,SIGNAL(triggered()),this,SLOT(finishCell()));
         tipToolBar->addAction(action);
         tipToolBar->addSeparator();
         ++it;
     }
}

void MainWindow::createTipToolBarActions(int column, QString str)
{
    if(sheet->getTipDirty() == true)
		tipMap = sheet->tipMap;
    destroyTipToolBarActions();
    QSet<QString> tipSet;
    QList<QString> listStr = tipMap.keys(column);
    //listStr contains the text in current column
    for(auto it = listStr.begin();it != listStr.end();++it)
    {
        if((*it).contains(str))
            tipSet.insert(*it);
        //if the text contains the str,add it to the tipSet
    }
    int count = tipSet.count();
    auto it = tipSet.begin();
    //display the contain of tipSet
    for(int i = 0;i < 10 && i<count;i++)
    {
        QAction *action = new QAction(this);
        action->setShortcut(tr("Alt+%1").arg(i));
        action->setText(tr("%1: ").arg(i)+*it);
        action->setData(*it);
        connect(action,SIGNAL(triggered()),this,SLOT(finishCell()));
        tipToolBar->addAction(action);
        tipToolBar->addSeparator();
        ++it;
    }//end for create actions
}

void MainWindow::finishCell()
{
    QAction *action = qobject_cast<QAction *>(sender());
    //transform QObject* to QAction*
    if(action)
    {
        QString str = action->data().toString();
        sheet->finish(str);
    }
}
