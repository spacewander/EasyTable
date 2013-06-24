#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include "ui_sortdialog.h"

class SortDialog : public Ui::SortDialog,public QDialog
{
    Q_OBJECT
public:
    explicit SortDialog(QWidget *parent = 0);
    void setColumnRange(QChar first,QChar last);
signals:
    
public slots:
};

#endif // SORTDIALOG_H
