/**
*@class FindDialog
*use this dialog for finding and replacing
*/
/**
*@file
*use this dialog for finding and replacing
*/
#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QCheckBox;
class QRadioButton;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = 0);
    
signals:
    // find with normal string
	/**
	*find forward
	*/
    void findNext(const QString &str,Qt::CaseSensitivity cs);
	/**
	*find backward
	*/
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
	/**
	*find from the begin of file to the end of file
	*/
    void findInAll(const QString &str,Qt::CaseSensitivity cs);
	/**
	*find from here and search the whole file
	*/
    void findFromHere(const QString &str,Qt::CaseSensitivity cs);

// find with regex
    /**
    *find forward using regex
    */
    void findNextRE(const QRegExp &re);
    /**
    *find backward using regex
    */
    void findPreviousRE(const QRegExp &re);
    /**
    *find from the begin of file to the end of file using regex
    */
    void findInAllRE(const QRegExp &re);
    /**
    *find from here and search the whole file using regex
    */
    void findFromHereRE(const QRegExp &re);

	/**
	*replace the context of the cell found
	*/
    void replaceSelectedCell(const QString &str);

public slots:
    void findClicked();
    void replaceClicked();
    void enableFindButton(const QString &text);
    void enableReplaceButton(const QString &text);
    void enableRegexCheckBox(bool ok);
    void enableCaseCheckBox(bool ok);

    void match();
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *replaceLabel;
    QLineEdit *replaceLineEdit;

    QCheckBox *caseCheckBox;
    QCheckBox *regexCheckBox;

    QRadioButton *backwardRadioButton;
    QRadioButton *forwardRadioButton;
    QRadioButton *findInAllRadioButton;
    QRadioButton *findFromHereRadioButton;
    QGroupBox *radioButtonGroupBox;

    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
