﻿//Qt 4.8.4版
#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QTableWidgetSelectionRange>
#include <QCheckBox>
#include <QSettings>
#include <QWidget>
#include <QToolBar>
#include <QCloseEvent>
#include <QEvent>
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
#include "easytable.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    sheet=new EasyTable;
    setCentralWidget(sheet);
	createActions();
	createMenus();
	createContextMenu();
	createStatusBar();
	createToolBars();
	readSettings();
	findDialog=0;
    setCurrentFile(tr("未命名"));
    setWindowModified(false);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    newAction=new QAction(tr("&新建"),this);
	newAction->setShortcut(QKeySequence::New);
	newAction->setStatusTip(tr("新建文件"));
	connect(newAction,SIGNAL(triggered()),this,SLOT(newFile()));
    openAction=new QAction(tr("&打开"),this);
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(tr("打开文件"));
	connect(openAction,SIGNAL(triggered()),this,SLOT(open()));//wrong here!should use slots to use LoadFile!
    saveAction=new QAction(tr("&保存"),this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("保存文件"));
	connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
    saveAsAction=new QAction(tr("另存为"),this);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("另存为"));
	connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));

	for(int i=0;i<MaxRecentFiles;i++)
	{
		recentFileActions[i]=new QAction(this);
		recentFileActions[i]->setVisible(false);
		connect(recentFileActions[i],SIGNAL(triggered()),
			this,SLOT(openRecentFile()));
	}

    closeAction=new QAction(tr("&关闭"),this);
	closeAction->setShortcut(QKeySequence::Close);
	closeAction->setStatusTip(tr("关闭"));
	connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));
    exitAction=new QAction(tr("退出"),this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("退出"));
	connect(exitAction,SIGNAL(triggered()),this,SLOT(closeAllWindows()));//this should be the QApplication;

    cutAction=new QAction(tr("剪切"),this);
	cutAction->setShortcut(QKeySequence::Cut);
	cutAction->setStatusTip(tr("剪切"));
    connect(cutAction,SIGNAL(triggered()),sheet,SLOT(cut()));
    copyAction=new QAction(tr("复制"),this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setStatusTip(tr("复制"));
    connect(cutAction,SIGNAL(triggered()),sheet,SLOT(copy()));
    pasteAction=new QAction(tr("粘贴"),this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("粘贴"));
    connect(cutAction,SIGNAL(triggered()),sheet,SLOT(paste()));
    deleteAction=new QAction(tr("删除"),this);
	deleteAction->setShortcut(QKeySequence::Delete);
	deleteAction->setStatusTip(tr("删除"));
    connect(cutAction,SIGNAL(triggered()),sheet,SLOT(del()));

    selectRowAction=new QAction(tr("选中行"),this);
	selectRowAction->setStatusTip(tr("选择行"));
    connect(selectRowAction,SIGNAL(triggered()),sheet,SLOT(selectCurrentRow()));
    selectColumnAction=new QAction(tr("选中列"),this);
	selectColumnAction->setStatusTip(tr("选择列"));
    connect(selectColumnAction,SIGNAL(triggered()),sheet,SLOT(selectCurrentColumn()));
    selectAllAction=new QAction(tr("选中全部"),this);
	selectAllAction->setShortcut(QKeySequence::SelectAll);
	selectAllAction->setStatusTip(tr("选择所有单元格"));
    connect(selectAllAction,SIGNAL(triggered()),sheet,SLOT(selectAll()));

    findAction=new QAction(tr("查找"),this);
	findAction->setStatusTip(tr("查找"));
	findAction->setShortcut(QKeySequence::Find);
	connect(findAction,SIGNAL(triggered()),this,SLOT(find()));
    goToCellAction=new QAction(tr("前往"),this);
	goToCellAction->setStatusTip(tr("前往"));
    goToCellAction->setShortcut(tr("Ctrl+T"));
	connect(goToCellAction,SIGNAL(triggered()),this,SLOT(goToCell()));

    recalculateAction=new QAction(tr("重新计算"),this);
    recalculateAction->setStatusTip(tr("recalculate al the cells in the sheet"));
    connect(recalculateAction,SIGNAL(triggered()),sheet,SLOT(recalculate()));
    sortAction=new QAction(tr("排序"),this);
    sortAction->setStatusTip(tr("Sort the cells in the sheet"));
    sortAction->setShortcut(tr("Shift+O"));
	connect(sortAction,SIGNAL(triggered()),this,SLOT(sort()));
    autoRecalcAction=new QAction(tr("自动重新计算"),this);
    autoRecalcAction->setStatusTip(tr("Set whether to recalculate the sheet automatically"));
    connect(autoRecalcAction,SIGNAL(triggered(bool)),sheet,SLOT(setAutoRecalculate(bool)));

    showGridAction=new QAction(tr("&显示边线"),this);
	showGridAction->setCheckable(true);
    showGridAction->setChecked(sheet->showGrid());
    showGridAction->setStatusTip(tr("Show or hide the grid of sheet"));
	connect(showGridAction,SIGNAL(toggled(bool)),
        sheet,SLOT(setShowGrid(bool)));

    aboutAction=new QAction(tr("&关于"),this);
	aboutAction->setStatusTip(tr("Show the about box"));
	connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

}
void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("文件"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);
	separatorAction=fileMenu->addSeparator();
	for(int i=0;i<MaxRecentFiles;i++)
	{
		fileMenu->addAction(recentFileActions[i]);
	}
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

    editMenu=menuBar()->addMenu(tr("编辑"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(deleteAction);

    selectSubMenu=editMenu->addMenu(tr("选中"));
	selectSubMenu->addAction(selectAllAction);
	selectSubMenu->addAction(selectRowAction);
	selectSubMenu->addAction(selectColumnAction);

	editMenu->addSeparator();
	editMenu->addAction(findAction);
	editMenu->addAction(goToCellAction);
    editMenu->addAction(sortAction);

    toolsMenu=menuBar()->addMenu(tr("工具"));
    //toolsMenu->addAction(recalculateAction);
	toolsMenu->addAction(sortAction);

    optionsMenu=menuBar()->addMenu(tr("选项"));
	optionsMenu->addAction(showGridAction);
    //optionsMenu->addAction(autoRecalcAction);

	menuBar()->addSeparator();

    helpMenu=menuBar()->addMenu(tr("帮助"));
	helpMenu->addAction(aboutAction);
}
void MainWindow::createContextMenu()
{
    sheet->addAction(cutAction);
    sheet->addAction(copyAction);
    sheet->addAction(pasteAction);
    sheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}
void MainWindow::createToolBars()
{
    fileToolBar=addToolBar(tr("文件"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
    fileToolBar->addSeparator();

    editToolBar=addToolBar(tr("编辑"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
	editToolBar->addAction(findAction);
	editToolBar->addAction(goToCellAction);
    editToolBar->addAction(sortAction);
}
void MainWindow::createStatusBar()
{
    locationLabel=new QLabel("W199");
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());

	formulaLabel=new QLabel;
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
void MainWindow::newFile()
{
	if(okToContinue())
	{
        sheet->clear();
		setCurrentFile("");
	}
}
bool MainWindow::okToContinue()
{
	if(isWindowModified())
	{
        int r=QMessageBox::warning(this,tr("sheet"),
			tr("The document has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(QMessageBox::Yes==r)
		{
			return save();
		}
		else
		{
            if(QMessageBox::Cancel==r)
				return false;
		}
	}
	return true;
}
void MainWindow::open()
{
	if(okToContinue())
	{
		QString fileName=QFileDialog::getOpenFileName(this,
            tr("打开表格" ),
			".",
            tr("sheet files (*.sh)\n"
			"Comma-separeted values files (*.csv)\n"
			"Microsoft Office Excel files (*.xls *.xlsx)"));//set the openable type?
		if(!fileMenu->isEmpty())
			loadFile(fileName);
	}
}
bool MainWindow::loadFile(const QString &fileName)
{
    if(!sheet->readFile(fileName))
	{
		statusBar()->showMessage(tr("Loading canceled"),2000);
		return false;
	}
	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"),2000);
	return true;
}
bool MainWindow::save()
{
	if(curFile.isEmpty())
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
		statusBar()->showMessage(tr("Saving canceled"),2000);
		return false;
	}
	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"),2000);
	return true;
}
bool MainWindow::saveAsFile(const QString &fileName)
{
    if(!sheet->writeFile(fileName))
	{
		statusBar()->showMessage(tr("Saving canceled"),2000);
		return false;
	}
	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"),2000);
	return true;
}
bool MainWindow::saveAs()
{
	QString fileName=QFileDialog::getSaveFileName(
        this,tr("保存表格"),
		".",
        tr("sheet files (*.sh)\n"
		"Comma-separeted values files (*.csv)\n"
		"Microsoft Office Excel files (*.xls *.xlsx)"));//set the save-able type?
	if(fileName.isEmpty())
	{
		return false;
	}
	return saveFile(fileName);
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
	}
}
void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile=fileName;
	setWindowModified(true);
    QString shownName=tr("未命名");
	if(!curFile.isEmpty())
	{
		shownName=strippedName(curFile);
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}
    setWindowTitle(tr("%1[*]-%2").arg(shownName).arg(tr("sheet")));
}
QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}
void MainWindow::updateRecentFileActions()
{
	QMutableStringListIterator i(recentFiles);
	while(i.hasNext())
	{
		if(!QFile::exists(i.next()))
			i.remove();
	}
	for(int j=0;j<MaxRecentFiles;j++)
	{
		if(j<recentFiles.count())
		{
			QString text=tr("&%1 %2").arg(j+1)
				.arg(strippedName(recentFiles[j]));
			recentFileActions[j]->setText(text);
			recentFileActions[j]->setData(recentFiles);
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
		QAction *action=qobject_cast<QAction *>(sender());
		if(action)
			loadFile(action->data().toString());
	}
}
void MainWindow::closeAllWindows()
{
		QAction *action=qobject_cast<QAction *>(sender());
		if(action)
			close();
}

void MainWindow::find()
{
	if(!findDialog)
	{
		findDialog=new FindDialog(this);
		connect(findDialog,SIGNAL(findNext(const QString&,
			Qt::CaseSensitivity)),
            sheet,SLOT(findNext(const QString&,
			Qt::CaseSensitivity)));
		connect(findDialog,SIGNAL(findPrevious(const QString&,
			Qt::CaseSensitivity)),
            sheet,SLOT(findPrevious(const QString&,
			Qt::CaseSensitivity)));

	}
	findDialog->show();
	findDialog->raise();
    findDialog->activateWindow();
}
void MainWindow::goToCell()
{
    GotoCellDialog toCell(this);
    if(toCell.exec())
	{
        QString str=toCell.lineEdit->text().toUpper();
        sheet->setCurrentCell(str.mid(1).toInt()-1,
			str[0].unicode()-'A');
	}
}
void MainWindow::sort()
{
	SortDialog dialog(this);
    QTableWidgetSelectionRange range=sheet->selectedRange();
	dialog.setColumnRange('A'+range.leftColumn(),
		'A'+range.rightColumn());
	if(dialog.exec())
	{
        EasyTableCompare compare;
		compare.keys[0]=
			dialog.primaryColumnCombo->currentIndex();
		compare.keys[1]=
			dialog.SecondaryColumnCombo->currentIndex()-1;
		compare.keys[2]=
			dialog.TertiaryColumnCombo->currentIndex()-1;
		compare.ascending[0]=
			(dialog.primaryOrderCombo->currentIndex()==0);
		compare.ascending[1]=
			(dialog.SecondaryOrderCombo->currentIndex()==0);
		compare.ascending[2]=
			(dialog.TertiaryOrderCombo->currentIndex()==0);
        sheet->sort(compare);
	}
}
void MainWindow::about()
{
	QMessageBox::about(this,tr("About EasyTable"),
		tr("<h2>EasyTable 0.1</h2>"
		"<p>Copyleft &copy; BugMore Software Inc."));
}
void MainWindow::writeSettings()
{
	QSettings settings("BugMore Software Inc.","EasyTable");
	settings.setValue("geometry",saveGeometry());
	settings.setValue("recentFiles",recentFiles);
	settings.setValue("showGrid",showGridAction->isChecked());
	settings.setValue("autoRecalc",autoRecalcAction->isChecked());
}
void MainWindow::readSettings()
{
	QSettings settings("BugMore Software Inc.","EasyTable");
	restoreGeometry(settings.value("geometry").toByteArray());
	recentFiles=settings.value("recentFiles").toStringList();
	updateRecentFileActions();
	bool showGrid=settings.value("showGrid",true).toBool();
	showGridAction->setChecked(showGrid);
	bool autoRecalc=settings.value("autoRecalc",true).toBool();
	autoRecalcAction->setChecked(autoRecalc);
}
//void MainWindow::newFile()
//{
//    MainWindow *mainWin=new MainWindow;
//    mainWin->show();
//}
