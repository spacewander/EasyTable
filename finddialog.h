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
    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
    void findInAll(const QString &str,Qt::CaseSensitivity cs);
    void findFromHere(const QString &str,Qt::CaseSensitivity cs);
    void replaceSelectedCell(const QString &str);
public slots:
    void findClicked();
    void replaceClicked();
    void enableFindButton(const QString &text);
    void enableReplaceButton(const QString &text);
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *replaceLabel;
    QLineEdit *replaceLineEdit;
    QCheckBox *caseCheckBox;

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
