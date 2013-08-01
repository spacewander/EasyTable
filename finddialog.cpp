#include "finddialog.h"
#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent)
{
    label = new QLabel(tr("内容"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    caseCheckBox = new QCheckBox(tr("匹配"));
    caseCheckBox->setShortcut(QKeySequence::ExactMatch);
    findButton = new QPushButton(tr("查找"));
    findButton->setShortcut(QKeySequence::Find);
    findButton->setDefault(true);
    findButton->setEnabled(false);
    backwardCheckBox = new QCheckBox(tr("查找前一个"));
    backwardCheckBox->setShortcut(QKeySequence::FindPrevious);
    closeButton=new QPushButton(tr("关闭"));
    closeButton->setShortcut(QKeySequence::Close);

    connect(lineEdit, SIGNAL(textChanged(const QString &)),this,
            SLOT(enableFindButton(const QString &)));
    connect(findButton, SIGNAL(clicked()),this, SLOT(findClicked()));
    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(backwardCheckBox);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("查找"));
    setFixedHeight(sizeHint().height());
}

void FindDialog::findClicked()
{
    QString text=lineEdit->text();
    Qt::CaseSensitivity cs=
            caseCheckBox->isChecked()?Qt::CaseSensitive
                                    :Qt::CaseInsensitive;
    if(backwardCheckBox->isChecked())
    {
        emit findPrevious(text,cs);
    }
    else
    {
        emit findNext(text,cs);
    }
}

void FindDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}
