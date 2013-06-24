#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class QLineEdit;

class GotoCellDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GotoCellDialog(QWidget *parent = 0);
    QLabel *label;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *lineEdit;
signals:
    
public slots:
    void on_lineEdit_textChanged();
};

#endif // GOTOCELLDIALOG_H
