﻿/**
*@class SortDialog
*the dialog is for sorting
*/
/**
*@file
*the dialog is for sorting
*/
#include "sortdialog.h"
#include "ui_sortdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
/**
*the constructor of SortDialog
*/
SortDialog::SortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortDialog)
{
    ui->setupUi(this);
    ui->secondaryGroupBox->hide();
    ui->tertiaryGroupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setColumnRange('A','Z');
}
/**
*the destructor of SortDialog
*/
SortDialog::~SortDialog()
{
    delete ui;
}
/**
*set the value range of columnCombos
*/
void SortDialog::setColumnRange(QChar first,QChar last)
{
    ui->primaryColumnCombo->clear();
    ui->secondaryColumnCombo->clear();
    ui->tertiaryColumnCombo->clear();
    ui->secondaryColumnCombo->addItem(tr("无"));
    ui->tertiaryColumnCombo->addItem(tr("无"));
    ui->primaryColumnCombo->setMinimumSize(
        ui->secondaryColumnCombo->sizeHint());
    QChar ch = first;
    while(ch <= last)
    {
        ui->primaryColumnCombo->addItem(QString(ch));
        ui->secondaryColumnCombo->addItem(QString(ch));
        ui->tertiaryColumnCombo->addItem(QString(ch));
        ch = ch.unicode()+1;
    }
}
/**
*initial the compare keys and the order
*/
void SortDialog::setSortKeyandAscending(EasyTableCompare &compare)
{
    //currentIndex begins with 0
    compare.keys[0] =
                ui->primaryColumnCombo->currentIndex();
    compare.keys[1] =
                ui->secondaryColumnCombo->currentIndex();
    compare.keys[2] =
                ui->tertiaryColumnCombo->currentIndex();
    compare.ascending[0] =
                (ui->primaryOrderCombo->currentIndex() == 0);
    compare.ascending[1] =
                (ui->secondaryOrderCombo->currentIndex() == 0);
    compare.ascending[2] =
                (ui->tertiaryOrderCombo->currentIndex() == 0);
}
