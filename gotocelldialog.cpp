#include "gotocelldialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QRegExp>
#include <QRegExpValidator>
#include <QLayout>

GotoCellDialog::GotoCellDialog(QWidget *parent) :
    QDialog(parent)
{
    okButton = new QPushButton(tr("好的"));
    cancelButton = new QPushButton(tr("取消"));
    cancelButton->setShortcut(QKeySequence::Close);
    okButton->setEnabled(false);
    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

    label = new QLabel(tr("前往:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,4}");
    //The limit of columns is 9999,I hope no one needs more columns
    //And the limit of rows is 26
    lineEdit->setValidator(new QRegExpValidator(regExp,this));
    connect(lineEdit,SIGNAL(textChanged(const QString &)),this,
                            SLOT(on_lineEdit_textChanged()));

    QHBoxLayout *highLayout = new QHBoxLayout;
    highLayout->addWidget(label);
    highLayout->addWidget(lineEdit);

    QHBoxLayout *lowLayout = new QHBoxLayout;
    lowLayout->addWidget(okButton);
    lowLayout->addWidget(cancelButton);
    lowLayout->addStretch(20);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(highLayout);
    mainLayout->addLayout(lowLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("前往"));
    setFixedHeight(sizeHint().height());
    setWindowOpacity(1.0);
}

void GotoCellDialog::on_lineEdit_textChanged()
{
    okButton->setEnabled(lineEdit->hasAcceptableInput());
}

int GotoCellDialog::getRow()
{
    QString str = lineEdit->text().toUpper();
    return  str.mid(1).toInt()-1;
}

int GotoCellDialog::getColumn()
{
    QString str = lineEdit->text().toUpper();
    return str[0].unicode() - 'A';
}
