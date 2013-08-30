/**
*@file
*use this dialog for going to special cell
*/
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
    int getRow();
    int getColumn();
signals:
    
public slots:
    void on_lineEdit_textChanged();
private:
    QLabel *label;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *lineEdit;
};

#endif // GOTOCELLDIALOG_H
