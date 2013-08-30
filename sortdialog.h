/**
*@file
*the dialog is for sorting
*/
#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include "easytable.h"

namespace Ui {
class SortDialog;
}

class SortDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SortDialog(QWidget *parent = 0);
    ~SortDialog();
    void setColumnRange(QChar first,QChar last);
    void setSortKeyandAscending(EasyTableCompare &compare);
private:
    Ui::SortDialog *ui;
};

#endif // SORTDIALOG_H
