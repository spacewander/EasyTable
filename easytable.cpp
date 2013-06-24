#include "easytable.h"
#include <QTableWidget>
#include <QFile>
#include <QMessageBox>
#include <QDataStream>
#include <QApplication>
#include <QClipboard>
#include <QStringList>
#include "cell.h"

EasyTable::EasyTable(QWidget *parent) :
    QTableWidget(parent)
{
    autoRecalc=true;

    setItemPrototype(new Cell);
    setSelectionMode(ContiguousSelection);

    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),
            this,SLOT(somethingChanged()));
    clear();
}

void EasyTable::clear()
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);

    for(int i=0;i<ColumnCount;i++)
    {
        QTableWidgetItem *item=new QTableWidgetItem;
        item->setText(QString(QChar('A'+i)));
        setHorizontalHeaderItem(i,item);
    }
    setCurrentCell(0,0);
}
Cell* EasyTable::cell(int row, int column) const
{
    return static_cast<Cell*>(item(row,column));
}
QString EasyTable::text(int row, int column) const
{
    Cell *c=cell(row,column);
    if(c)
    {
        return c->text();
    }
    else
    {
        return "";
    }
}
QString EasyTable::formula(int row, int column) const
{
    Cell *c=cell(row,column);
    if(c)
    {
        return c->formula();
    }
    else
    {
        return "";
    }
}
void EasyTable::setFormula(int row, int column, const QString &formula)
{
    Cell *c=cell(row,column);
    if(!c)
    {
        c=new Cell;
        setItem(row,column,c);
    }
    c->setFormula(formula);
}
QString EasyTable::currentLocation()const
{
    return QChar('A'+currentColumn())
            +QString::number(currentRow()+1);
}
QString EasyTable::currentFormula()const
{
    return formula(currentRow(),currentColumn());
}
void EasyTable::somethingChanged()
{
    if(autoRecalc)
        recalculate();
    emit modified();
}
bool EasyTable::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("EasyTable"),
                             tr("不能写入文件 %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);//version is 4.8
    out<<quint32(MagicNumber);//if quint64? What about MagicNumber
    //quint/qint强制转化数据大小，使数据获得应有的大小。MagicNumber是一个位于文件开始
    //的随机数，确定文件格式
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row=0;row<RowCount;row++)
    {
        for(int column=0;column<ColumnCount;column++)
        {
            QString str=formula(row,column);
            if(!str.isEmpty())
                out<<quint16(row)<<quint16(column)<<str;
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}
bool EasyTable::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("EasyTable"),
                             tr("不能读取文件 %1:\n%2")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);//version is 4.8
    quint32 magic;
    in>>magic;
    if(magic!=MagicNumber)
    {
        QMessageBox::warning(this,tr("EasyTable"),
                             tr("EasyTable程序不能处理文件 %1 ")
                             .arg(file.fileName()));
        return false;
    }
    clear();
    quint16 row;
    quint16 column;
    QString str;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd())
    {
        in>>row>>column>>str;
        setFormula(row,column,str);
    }
    QApplication::restoreOverrideCursor();
    return true;
}

QTableWidgetSelectionRange EasyTable::selectedRange()const
{
    QList<QTableWidgetSelectionRange> ranges=selectedRanges();
    if(ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}
void EasyTable::cut()
{
    copy();
    del();
}
void EasyTable::copy()
{
    QTableWidgetSelectionRange range=selectedRange();
    QString str;
    for(int i=0;i<range.rowCount();i++)
    {
        if(i>0)
            str+='\n';
        for(int j=0;j<range.columnCount();j++)
        {
            if(j>0)
                str+='\t';
            str+=formula(range.topRow()+i,range.leftColumn()+j);
        }
    }
    QApplication::clipboard()->setText(str);
}
void EasyTable::paste()
{
    QTableWidgetSelectionRange range=selectedRange();
    QString str=QApplication::clipboard()->text();
    QStringList rows=str.split('\n');
    int numRows=rows.count();
    int numColumns=rows.first().count('\t')+1;
    if(range.rowCount()*range.columnCount()!=1
            &&(range.rowCount()!=numRows
               ||range.columnCount()!=numColumns))
    {
        QMessageBox::information(this,tr("EasyTable"),
                                 tr("数据无法复制或粘贴"
                                   "因为数据复制或粘贴"
                                   "的区域大小不合适"));//
        return;
    }
    for(int i=0;i<numRows;i++)
    {
        QStringList columns=rows[i].split('\t');
        for(int j=0;j<numColumns;j++)
        {
            int row=range.topRow()+i;
            int column=range.leftColumn()+j;
            if(row<RowCount&&column<ColumnCount)
                setFormula(row,column,columns[j]);
        }
    }
    somethingChanged();
}
void EasyTable::del()
{
    QList<QTableWidgetItem *> items=selectedItems();
    if(!items.isEmpty())
    {
        foreach (QTableWidgetItem *item,items)
            delete item;
        somethingChanged();
    }
}
void EasyTable::selectCurrentRow()
{
    selectRow(currentRow());
}
void EasyTable::selectCurrentColumn()
{
    selectColumn(currentColumn());
}
void EasyTable::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row=currentRow();
    int column=currentColumn()+1;
    while(row<RowCount)
    {
        while(column<ColumnCount)
        {
            if(text(row,column).contains(str,cs))
            {
                clearSelection();
                setCurrentCell(row,column);
                activateWindow();
                return;
            }
            column++;
        }
        column=0;
        row++;
    }
    QApplication::beep();
}
void EasyTable::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    int row=currentRow();
    int column=currentColumn()-1;
    while(row>=0)
    {
        while(column>=0)
        {
            if(text(row,column).contains(str,cs))
            {
                clearSelection();
                setCurrentCell(row,column);
                activateWindow();
                return;
            }
            column--;
        }
        column=ColumnCount-1;
        row--;
    }
    QApplication::beep();
}
void EasyTable::recalculate()
{
    for(int column=0;column<ColumnCount;column++)
    {
        for(int row=0;row<RowCount;row++)
        {
            if(cell(row,column))
                cell(row,column)->setDirty();
        }//end for
    }//end for
    viewport()->update();
}
void EasyTable::setAutoRecalculate(bool recalc)
{
    autoRecalc=recalc;
    if(autoRecalc)
        recalculate();
}
void EasyTable::sort(const EasyTableCompare &compare)
{
    QList<QStringList> rows;
    QTableWidgetSelectionRange range=selectedRange();
    int i;
    for(i=0;i<range.rowCount();i++)
    {
        QStringList row;
        for(int j=0;j<range.columnCount();j++)
            row.append(formula(range.topRow()+i,
                               range.leftColumn()+j));
        row.append(row);
    }
    qStableSort(rows.begin(),rows.end(),compare);
    for(i=0;i<range.rowCount();i++)
    {
        for(int j=0;j<range.columnCount();j++)
            setFormula(range.topRow()+1,range.leftColumn()+j,
                       rows[i][j]);
    }
    clearSelection();
    somethingChanged();
}

bool EasyTableCompare::operator ()(const QStringList& row1,
                                     const QStringList& row2) const
{
    for(int i=0;i<KeyCount;i++)
    {
        int column=keys[i];
        if(column!=-1)
        {
            if(row1[column]!=row2[column])
            {
                if(ascending[i])
                {
                    return row1[column]<row2[column];
                }
                else
                {
                    return row1[column]>row2[column];
                }//end else
            }//end if row1[column]!=row2[column]
        }//end if column!=-1
    }//end for
    return false;
}
