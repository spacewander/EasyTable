/**
*@file
*use this dialog for finding and replacing
*/
#include "finddialog.h"

#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
/**
*the constructor of FindDialog
*/
FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent)
{
    label = new QLabel(tr("内容"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    replaceLabel = new QLabel(tr("替换为"));
    replaceLineEdit = new QLineEdit;
    replaceLabel->setBuddy(replaceLineEdit);

    caseCheckBox = new QCheckBox(tr("大小写匹配"));
    caseCheckBox->setShortcut(QKeySequence::ExactMatch);

    findButton = new QPushButton(tr("查找"));
    findButton->setShortcut(QKeySequence::Find);
    findButton->setDefault(true);
    findButton->setEnabled(false);
    replaceButton = new QPushButton(tr("替换"));
    replaceButton->setEnabled(false);
    closeButton=new QPushButton(tr("关闭"));
    closeButton->setShortcut(QKeySequence::Close);

    backwardRadioButton = new QRadioButton(tr("向前查找"));
    backwardRadioButton->setShortcut(QKeySequence::FindPrevious);
    forwardRadioButton = new QRadioButton(tr("向后查找"));
    forwardRadioButton->setShortcut(QKeySequence::FindNext);
    findInAllRadioButton = new QRadioButton(tr("全文件查找"));
    findFromHereRadioButton = new QRadioButton(tr("从当前开始全文查找"));
    findFromHereRadioButton->setChecked(true);
    findFromHereRadioButton->setShortcut(QKeySequence::Find);

    radioButtonGroupBox = new QGroupBox(tr("查找条件组"));
    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(backwardRadioButton);
    groupBoxLayout->addWidget(forwardRadioButton);
    groupBoxLayout->addWidget(findInAllRadioButton);
    groupBoxLayout->addWidget(findFromHereRadioButton);
    radioButtonGroupBox->setLayout(groupBoxLayout);


    connect(lineEdit, SIGNAL(textChanged(const QString &)),this,
            SLOT(enableFindButton(const QString &)));
    connect(replaceLineEdit, SIGNAL(textChanged(const QString &)),this,
            SLOT(enableReplaceButton(const QString &)));
    connect(findButton, SIGNAL(clicked()),this, SLOT(findClicked()));
    connect(replaceButton, SIGNAL(clicked()),this, SLOT(replaceClicked()));
    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));

    QVBoxLayout *topLeftLayout = new QVBoxLayout;
    QHBoxLayout *findLayout = new QHBoxLayout;
    findLayout->addWidget(label);
    findLayout->addWidget(lineEdit);
    QHBoxLayout *replaceLayout = new QHBoxLayout;
    replaceLayout->addWidget(replaceLabel);
    replaceLayout->addWidget(replaceLineEdit);
    topLeftLayout->addLayout(findLayout);
    topLeftLayout->addLayout(replaceLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(radioButtonGroupBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(replaceButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("查找"));
    setFixedHeight(sizeHint().height());
}
/**
*when findButton is clicked,emit particular singal to MainWindow
*and set the dialog translucent
*/
void FindDialog::findClicked()
{
    QString text = lineEdit->text();
    Qt::CaseSensitivity cs =
            caseCheckBox->isChecked() ? Qt::CaseSensitive
                                      : Qt::CaseInsensitive;
    if(backwardRadioButton->isChecked())
    {
        emit findPrevious(text,cs);
    }
    else if(forwardRadioButton->isChecked())
    {
        emit findNext(text,cs);
    }
    else if(findFromHereRadioButton->isChecked())
    {
        emit findFromHere(text,cs);
    }
    else
    {
        emit findInAll(text,cs);
    }
    setWindowOpacity(0.5);
}
/**
*enable FindButton if there is some input,and set the dialog opaque
*/
void FindDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
    setWindowOpacity(1);
}
/**
*emit the signal to replace,and set the dialog opaque
*/
void FindDialog::replaceClicked()
{
    emit replaceSelectedCell(replaceLineEdit->text());
    setWindowOpacity(1);
}
/**
*enable replaceButton if there is some input,and findButton is enabled
*/
void FindDialog::enableReplaceButton(const QString &text)
{
    replaceButton->setEnabled(!text.isEmpty() && findButton->isEnabled());
}
