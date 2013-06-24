#include "sortdialog.h"

SortDialog::SortDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    secondaryGroupBox->hide();
    tertiaryGroupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setColumnRange('A','Z');
}

void SortDialog::setColumnRange(QChar first,QChar last)
{
    primaryColumnCombo->clear();
    SecondaryColumnCombo->clear();
    TertiaryColumnCombo->clear();
    SecondaryColumnCombo->addItem(tr("无"));
    TertiaryColumnCombo->addItem(tr("无"));
    primaryColumnCombo->setMinimumSize(
        SecondaryColumnCombo->sizeHint());
    QChar ch=first;
    while(ch<=last)
    {
        primaryColumnCombo->addItem(QString(ch));
        SecondaryColumnCombo->addItem(QString(ch));
        TertiaryColumnCombo->addItem(QString(ch));
        ch=ch.unicode()+1;
    }
}
