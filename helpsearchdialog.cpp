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

   // int len = actionList.length();
   /* for(int i = 0; i < len; ++i)
    {
        setTable(i,actionList.at(i));
    }
*/
    connect(okButton,SIGNAL(clicked()),this,SLOT(demoAction()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(lineEdit,SIGNAL(textChanged(QString)),this,
            SLOT(changeActionList(QString)));
}

/**
 * change the list in table while the text in lineEdit has changed and is valid
 * @param QString str
 */
void HelpSearchDialog::changeActionList(QString str)
{
    if(str.isEmpty() || str == "/")
        return;
    QStringList strList = str.split(" ");    //replace all blank
    okButton->setEnabled(true);
    int i = 0;
    auto constEnd = actionList.constEnd();
    bool ok = true;
    for(auto it = actionList.constBegin(); it != constEnd; ++it)
    {
        for(auto strIt = strList.constBegin(); strIt != strList.constEnd(); ++strIt)
        {
            if( !(*it).contains(*strIt) )
            {
                ok = false;
                break;
            }
        }
        if(ok)
        {
            setTable(i,*it);
            ++i;
        }
        ok = true;
    }
    while(i < ITEMS)
    {
        setTable(i,"");
        ++i;
    }
}
/**
 * select the target in table and emit its text
 */
void HelpSearchDialog::demoAction()
{
    QTableWidgetSelectionRange selectRange;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    if(!ranges.isEmpty())
         selectRange = ranges.first();
    QString str = "";
    QTableWidgetItem *item = table->item(selectRange.topRow(),selectRange.leftColumn());
    if(item != nullptr)
        str = item->text();
    triggerMenu(str);
}
/**
 * simply emit the string to trigger selected Menu
 * @param QString str
 */
void HelpSearchDialog::triggerMenu(QString &str)
{
    this->close();
    emit actionSelected(str);
}
/**
 * set the value of table in specular row with text.
 * Change the cell of table to uneditable
 * @param int row
 * @param QString s
 */
void HelpSearchDialog::setTable(int row, const QString &s)
{
    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(s);
    item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);//make it unable to edit
    table->setItem(row,0,item2);
}
