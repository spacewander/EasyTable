/**
*@file
*the MainWindow contains menubar,toolBar,and statusBar and so on
*it is controller of almost all the functions
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class QAction;
class QLabel;
class QIcon;
class FindDialog;
class EasyTable;
class GotoCellDialog;
class Find;
class HelpSearchDialog;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    friend class MainWidget;
    //I have to declare MainWidget as the friend class of MainWindow,
    //so that MainWidget is able to call the private methods of MainWindow

protected:
    void closeEvent(QCloseEvent* event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void updateRecentFileActions();
    void updateRecentFiles(QString &fileName);
    void exitApplication();
    void closeSubWindow();
    void closeCancelled();
    void showToolBar();
    void hideToolBar();

public slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void closeWindow();
    void openRecentFile();
    void closeAllWindow();
    void print();
    void setAutoSave(bool ok);

private slots:
    void updateStatusBar();
    void sheetModified();
    void saveAfterTimeInterval();
    void createTipToolBar(bool ok);
    void refreshTipTool();

    void find();
    void goToCell();
    void sort();

    void about();
    void helpSearch();

    void setFont();
    void setTextColor();
    void setBackgroundColor();

    void setLeftAlignment();
    void setCenterAlignment();
    void setRightAlignment();
    void setTopAlignment();
    void setBottomAlignment();

    void setSolidGrid();
    void setDotGrid();
    void setDashGrid();
    void hideGrid();
    void showGrid();

    void createGroupByToolBar();
    void hideSpecificRow();
    void cancellGroupBy();
    void finishCell();

     void triggerMenu(QString& str);

private:
    void createActions();
    void createRecentFileActions();
    void createFileActions();
    void createEditActions();
    void createOptionActions();
    void createToolActions();
    void createFormulaActions();
    void createOtherActions();

    void createSubMenus();

    void createFileMenus();
    void createEditMenus();
    void createToolsMenus();
    void createOptionsMenus();
    void createHelpMenus();
    void createMenus();

    void createContextMenu();

    void createToolBars();
    void createStatusBar();
    void createGroupByToolBarView();
    void createTipToolBarView();

    QStringList& listActions();
    void searchActions(QMenu *menu, QStringList& list,QString topMenuName = "");
    void triggerActions(QMenu *menu,QStringList& list,int level);
    void popupMenu(QMenu *menu, QPoint location = QPoint(0,0) );

    void readSettings();
    void writeSettings();

    void createTipToolBarActions(int column);
    void createTipToolBarActions();
    void createTipToolBarActions(int column,QString str);
    void destroyTipToolBarActions();

    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void refreshRecentFileActions();
    QString strippedName(const QString &fullFileName);

    EasyTable *sheet;
    FindDialog *findDialog;
    Find *findController;
    GotoCellDialog *toCell;
    HelpSearchDialog *helpDialog;
    QStringList actionList;

    QVector<int> maxRow;
    QMap<QString,int> tipMap;/// key is the text of cell,value is the column of cell

	/// timer is used to record how many seconds have passed
    QTimer *timer;
    bool autoSave;

    QLabel *locationLabel;
    QLabel *formulaLabel;

    QStringList recentFiles;
    QString curFile;

    QIcon textColorIcon;
    QIcon backgroundColorIcon;
    QIcon &setIconColor(QIcon &icon, QColor color);

	/// should be changed to be the same as the mainwidget
    enum{MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;
    QMenu *recentFilesSubMenu;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QMenu *alignmentSubMenu;
    QMenu *insertSubMenu;
    QMenu *chooseSubMenu;
    QMenu *formatSubMenu;
    QMenu *removeSubMenu;
    QMenu *hideSubMenu;
    QMenu *gridStyleSubMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *formatToolBar;
    QToolBar *alignmentToolBar;
    QToolBar *gridStyleToolBar;
    QToolBar *groupByToolBar;
    QToolBar *tipToolBar;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *exitAction;
    QAction *printAction;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *normalizeAction;

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;

    QAction *insertRowAction;
    QAction *insertColumnAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    QAction *removeRowAction;
    QAction *removeColumnAction;
    QAction *hideRowsAction;
    QAction *hideColumnsAction;
    QAction *showHiddenRangesAction;

    QAction *findAction;
    QAction *goToCellAction;
    QAction *recalculateAction;
    QAction *sortAction;
    QAction *groupByAction;
    QAction *hideGroupByAction;
    QAction *functionAction;

    QAction *autoRecalcAction;
    QAction *showGridAction;
    QAction *defaultAlignmentAction;
    QAction *autoResizeAction;
    QAction *autoSaveAction;
    QAction *autoTipAction;
    QAction *showMainToolBarAction;
    QAction *hideMainToolBarAction;

    QAction *aboutAction;
    QAction *helpSearchActon;

    QAction *solidGridStyleAction;
    QAction *dotGridStyleAction;
    QAction *dashGridStyleAction;
    QAction *noneGridStyleAction;

    QAction *fontAction;
    QAction *textColorAction;
    QAction *textColorIconAction;
    QAction *backgroundColorIconAction;
    QAction *backgroundColorAction;

    QAction *leftAlignmentAction;
    QAction *centerAlignmentAction;
    QAction *rightAlignmentAction;
    QAction *topAlignmentAction;
    QAction *bottomAlignmentAction;
};

#endif // MAINWINDOW_H
