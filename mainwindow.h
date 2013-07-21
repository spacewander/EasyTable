#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QAction;
class QLabel;
class FindDialog;
class EasyTable;
class GotoCellDialog;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
protected:
	void closeEvent(QCloseEvent* event);
	private slots:
		void newFile();
		void open();
		bool save();
		bool saveAs();
		void find();
		void goToCell();
		void sort();
		void about();
		void openRecentFile();
		void closeAllWindows();
		void updateStatusBar();
        void sheetModified();
        void setFont();
        void setLeftAlignment();
        void setCenterAlignment();
        void setRightAlignment();
        void setTextColor();
        void setBackgroundColor();
private:
	void createActions();
	void createMenus();
	void createContextMenu();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	bool okToContinue();
	bool loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	bool saveAsFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);
    EasyTable *sheet;
	FindDialog *findDialog;
    GotoCellDialog *toCell;
	QLabel *locationLabel;
	QLabel *formulaLabel;
    QStringList recentFiles;
	QString curFile;

    enum{MaxRecentFiles = 5};
	QAction *recentFileActions[MaxRecentFiles];
	QAction *separatorAction;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *selectSubMenu;
	QMenu *toolsMenu;
	QMenu *optionsMenu;
	QMenu *helpMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
    QToolBar *formatToolBar;

	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *closeAction;
	QAction *exitAction;
    QMenu *recentFilesSubMenu;

	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *deleteAction;

    QAction *insertRowAction;
    QAction *insertColumnAction;
	QAction *selectRowAction;
	QAction *selectColumnAction;
	QAction *selectAllAction;

	QAction *findAction;
	QAction *goToCellAction;
	QAction *recalculateAction;
	QAction *sortAction;

	QAction *autoRecalcAction;
	QAction *showGridAction;

	QAction *aboutAction;

    QAction *fontAction;
    QAction *textColorAction;
    QAction *backgroundColorAction;
    QAction *leftAlignmentAction;
    QAction *centerAlignmentAction;
    QAction *rightAlignmentAction;
};

#endif // MAINWINDOW_H
