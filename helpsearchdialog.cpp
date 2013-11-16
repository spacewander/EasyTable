#include "helpsearchdialog.h"
#include <QStringList>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>

/**
 * @brief HelpSearchDialog::HelpSearchDialog
 *the constructor of HelpSearchDialog
 * @param parent : the base of helpSearchDialog
 * @param strList  : the action list of MainWindow
 */
HelpSearchDialog::HelpSearchDialog(QWidget *parent, QStringList &strList) :
    QDialog(parent),actionList(strList)
{
    lineLabel = new QLabel(tr("项目"));
    lineEdit = new QLineEdit;
    lineLabel->setBuddy(lineEdit);
    textLabel = new QLabel(tr("可选项"));
    table = new QTableWidget(ITEMS,1,this);
    table->setColumnWidth(0,table->width() * 2);
    textLabel->setBuddy(table);

    okButton = new QPushButton(tr("确定"));
    okButton->setDefault(true);
    okButton->setEnabled(false);
    cancelButton = new QPushButton(tr("退出"));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(lineLabel);
    topLayout->addWidget(lineEdit);
    QVBoxLayout *middleLayout = new QVBoxLayout;
    middleLayout->addWidget(textLabel);
    middleLayout->addWidget(table);
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(okButton);
    bottomLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("帮助搜索"));
    setFixedHeight(sizeHint().height());

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(tr("功能名"));
    table->setHorizontalHeaderItem(0,item);

    int len = actionList.length();
    for(int i = 0; i < len; ++i)
    {
        setTable(i,actionList.at(i));
    }

    connect(okButton,SIGNAL(clicked()),this,SLOT(demoAction()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(lineEdit,SIGNAL(textChanged(const QString& )),this,
            SLOT(changeActionList(const QString&)));
}

void HelpSearchDialog::changeActionList(const QString &str)
{
    int i = 0;
    for(auto it = actionList.constBegin(); it != actionList.constEnd(); ++it)
    {
        if((*it).contains(str))
        {
            setTable(i,*it);
            ++i;
        }
    }
    while(i < table->rowCount())
    {
        setTable(i,"");
        ++i;
    }
}
//to-do
/*
 *使用储存action列表
 *调用triggerMenu显示匹配的action
 *通过点击来选择(默认第一个)
 *发射action的url
 */
void HelpSearchDialog::demoAction()
{
   // triggerMenu();
}

void HelpSearchDialog::triggerMenu(QString &str)
{
    emit actionSelected(str);
}

void HelpSearchDialog::setTable(int row, const QString &s)
{
    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(s);
    table->setItem(row,0,item2);
}
