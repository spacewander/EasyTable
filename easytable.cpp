#include "easytable.h"
#include "easytablecompare.h"
#include "cell.h"
//recommand to use <QtGui>
#include <QApplication>
#include <QClipboard>
#include <QDataStream>
#include <QFile>
#include <QFont>
#include <QInputDialog>

#include <QMessageBox>
#include <QStringList>
#include <QtAlgorithms>
#include <QTableWidget>
#include <QTextDocument>
#include <QTextStream>

EasyTable::EasyTable(QWidget *parent) :
    QTableWidget(parent)
{
    autoRecalc = true;
    defaultAlignment = true;
    autoResize = true;
    RowCount = 32;      //set the number of rows
    ColumnCount = 18;   //set the number of columns
    setItemPrototype(new Cell);
    setSelectionMode(ExtendedSelection);
    clear();
    storeCellSize(0,0);//store the size of a normal cell;
    //Connections should be done later than clear(),
    //otherwise the size of Cell will be changed unexpectedly
    connectSignalsAndSlots();
}

void EasyTable::connectSignalsAndSlots()
{
    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),
            this,SLOT(somethingChanged()));
    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),
            this,SLOT(resizeCell(QTableWidgetItem*)));
}

void EasyTable::setHeaderItem()
{
    for(int i = 0;i<ColumnCount;i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString(QChar('A'+i)));
        setHorizontalHeaderItem(i,item);
    }
}

void EasyTable::storeCellSize(int row,int column)
{
    Width = columnWidth(column);
    Height = rowHeight(row);
}

void EasyTable::clear()
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);
    if(defaultAlignment == false)
    {
        for(int i = 0;i<RowCount;i++)
        {
            for(int j = 0;j<ColumnCount;j++)
            {
                setFormula(i,j,"");
                Cell *c = cell(i,j);
                if(c != nullptr)
                {
                    c->setDefaultAlignment(false);
                }
            }
        }
    }
    else
    {
        for(int i = 0;i<RowCount;i++)
        {
            for(int j = 0;j<ColumnCount;j++)
                setFormula(i,j,"");
        }
    }
    setHeaderItem();
    setCurrentCell(0,0);
}

Cell* EasyTable::cell(int row, int column) const
{
    return static_cast<Cell*>(item(row,column));
}

QString EasyTable::text(int row, int column) const
{
    Cell *c = cell(row,column);
    // if c is not NULL,return the string type of its data
    // ignoring the result of its formula
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
    Cell *c = cell(row,column);
    // if c is not NULL,return the string type of its data
    // considering the result of its formula
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
    Cell *c = cell(row,column);
    if(!c)
    {
        c = new Cell;
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
    QStringList fileInfo = fileName.split('.');
    if(fileInfo.count() != 2)
    {
        QMessageBox::warning(this,tr("EasyTable"),
                             tr("文件 %1 的类型无法识别\n")
                             .arg(fileName));
        return false;
    }
    int end = fileInfo.count() - 1;
    if(fileInfo.at(end) == "txt")
    {
        if(saveFileAsTxt(fileName))
            return true;
        else
        {
            QMessageBox::warning(this,tr("EasyTable"),
                                 tr("不能保存文本文件 %1\n")
                                 .arg(fileName));
            return false;
        }
    }
    else if(fileInfo.at(end) == "et")
    {
        if(saveFileAsEt(fileName))
            return true;
        else
        {
            QMessageBox::warning(this,tr("EasyTable"),
                                 tr("不能保存EasyTable文件 %1\n")
                                 .arg(fileName));
            return false;
        }//end else
    }//end if
    else if(fileInfo.at(end) == "csv")
    {
        if(saveFileAsCsv(fileName))
            return true;
        else
        {
            QMessageBox::warning(this,tr("EasyTable"),
                                 tr("不能保存EasyTable文件 %1\n")
                                 .arg(fileName));
            return false;
        }//end else
    }//end if
    QMessageBox::warning(this,tr("EasyTable"),
                         tr("文件 %1 的类型无法识别\n")
                         .arg(fileName));
    return false;
}

bool EasyTable::saveFileAsTxt(const QString &fileName)//not completed
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
    QTextStream out(&file);
    QString text;
    int r = QMessageBox::warning(this,tr("EasyTable"),
                         tr("是否在文本文件中用分割符来替换空单元格？"),
                         QMessageBox::Yes|QMessageBox::No);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(r == QMessageBox::Yes)
    {
        for(int row = 0;row<RowCount;row++)
        {
            for(int column = 0;column<ColumnCount;column++)
            {
                QString str = formula(row,column);
                if(str.isEmpty())
                    text += '\t';
                text += (str + '\t');
            }
            text += '\n';
        }
    }
    else
    {
        for(int row = 0;row<RowCount;row++)
        {
            for(int column = 0;column<ColumnCount;column++)
            {
                QString str = formula(row,column);
                text += (str + '\t');
            }
            text += '\n';
        }
    }
    for(int i = text.size();i > 0 ;i--)
    {
        if(text[i-1] != '\t' && text[i-1] != '\n')
        {
            text.truncate(i);
            break;
        }
    }
    QMessageBox::warning(this,"e",text);
    out<<text;
    QApplication::restoreOverrideCursor();
    return true;
}

bool EasyTable::saveFileAsEt(const QString &fileName)
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
    out<<quint32(RECOGNITIONNUMBER);
    //quint/qint transforms data size compulsory to its appropriate size.
    //RECOGNITIONNUMBER is a magicnumber in the beginning of file,
    //which decides the type of file.
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row = 0;row<RowCount;row++)
    {
        for(int column = 0;column<ColumnCount;column++)
        {
            QString str = formula(row,column);
            if(!str.isEmpty())
                out<<quint16(row)<<quint16(column)<<str;
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool EasyTable::saveFileAsCsv(const QString &fileName)//not completed
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
    QTextStream out(&file);
    QString text;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row = 0;row<RowCount;row++)
    {
        for(int column = 0;column<ColumnCount;column++)
        {
                QString str = formula(row,column);
                text += (str + ',');
         }
    }
    for(int i = text.size();i > 0 ;i--)
    {
        if(text[i-1] != ',')
        {
            text.truncate(i);
            break;
        }
    }
    QMessageBox::warning(this,"e",text);
    out<<text;
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
    quint32 recognitionNumber;
    in>>recognitionNumber;
    if(recognitionNumber != RECOGNITIONNUMBER)
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
        if(row >= RowCount || column >= ColumnCount)
        {
            clear();
            ColumnCount = (ColumnCount+5 > 26 ? 26 : ColumnCount+15);
            RowCount += 50;
            readFile(fileName);
        }
        setFormula(row,column,str);
    }
    QApplication::restoreOverrideCursor();
    return true;
}

QTextDocument *EasyTable::getContextForPrint()
{
    QString str;
    for(int i = 0;i<RowCount;i++)
    {
        if(i>0)
            str += '\n';
        for(int j = 0;j<ColumnCount;j++)
        {
            if(j>0)
                str += '\t';
            str += formula(i,j);
        }//end for column
    }//end for row
    context = new QTextDocument(str,this);
    return context;
}

QTableWidgetSelectionRange EasyTable::selectedRange()const
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if(ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}

void EasyTable::resizeCell(QTableWidgetItem *item)
{
    int column = item->column();
    int row = item->row();
    QFont font = item->font();
    qreal fontPointSizeF = font.pointSizeF();
    int fontPixelSize = font.pixelSize();
    qreal width = 0;
    qreal height = 0;
    double widthFactor = 1;
    double heightFactor = 1.5;
    if(fontPointSizeF >= 0)
    {
         width = item->text().size()*fontPointSizeF*widthFactor;
         height = fontPointSizeF*heightFactor;
    }
    else
    if(fontPixelSize >= 0)
    {
        width = item->text().size()*fontPixelSize*widthFactor;
        height = fontPixelSize*heightFactor;
    }
    cellSizeChange(row,column,width,height);
}

void EasyTable::cut()
{
    copy();
    del();
}

void EasyTable::copy()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str;
    for(int i = 0;i<range.rowCount();i++)
    {
        if(i>0)
            str += '\n';
        for(int j = 0;j<range.columnCount();j++)
        {
            if(j>0)
                str += '\t';
            str += formula(range.topRow()+i,range.leftColumn()+j);
        }//end for column
    }//end for row
    QApplication::clipboard()->setText(str);
}

void EasyTable::paste()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t')+1;
    if(range.rowCount()*range.columnCount() != 1
            &&(range.rowCount() != numRows
               ||range.columnCount() != numColumns))
    {
        QMessageBox::information(this,tr("EasyTable"),
                                 tr("数据无法复制或粘贴\n"
                                   "因为数据复制或粘贴"
                                   "的区域大小不合适"));
        return;
    }
    for(int i = 0;i < numRows;i++)//out of index!why?
    {
        QStringList columns = rows[i].split('\t');
        numColumns = columns.count();
        for(int j = 0;j < numColumns;j++)
        {
            int row = range.topRow()+i;
            int column = range.leftColumn()+j;
            if(row < RowCount && column < ColumnCount)
                setFormula(row,column,columns[j]);
        }//end for column
    }//end for row
    somethingChanged();
}

void EasyTable::del()
{
    QList<QTableWidgetItem *> items = selectedItems();
    if(!items.isEmpty())
    {
        foreach (QTableWidgetItem *item,items)
            item->setText("");
        somethingChanged();
    }
}

void EasyTable::rowInsert()
{
    insertRow(currentRow());
    //currenRow is the column we choose
    RowCount += 1;
    int i = currentRow() - 1;
    for(int j = 0;j<ColumnCount;j++)
    {
        setFormula(i,j,"");
        Cell *c = cell(i,j);
        if(!c)
            c->setDefaultAlignment(defaultAlignment);
    }
}

void EasyTable::columnInsert()
{
    if(ColumnCount >= 26)
    {
      QMessageBox::information(this,tr("EasyTable"),
                               tr("无法插入新列\n"
                                 "因为预定的列数已用完"));
      return;
    }
    insertColumn(currentColumn());
    //currentColumn is the column we choose
    ColumnCount += 1;
    setHeaderItem();
    int j = currentColumn() - 1;
    for(int i = 0;i<RowCount;i++)
    {
        setFormula(i,j,"");
        Cell *c = cell(i,j);
        if(!c)
            c->setDefaultAlignment(defaultAlignment);
    }
}

void EasyTable::rowRemove()
{
    removeRow(currentRow());
    RowCount--;
}

void EasyTable::columnRemove()
{
    removeColumn(currentColumn());
    ColumnCount--;
    setHeaderItem();
}

void EasyTable::rowHide()
{
    QTableWidgetSelectionRange range = selectedRange();
    int rows = range.rowCount();
    int firstRow = range.topRow();
    for(int i = 0;i<rows;i++)
        hideRow(i+firstRow);
}

void EasyTable::columnHide()
{
    QTableWidgetSelectionRange range = selectedRange();
    int columns = range.columnCount();
    int firstColumn = range.leftColumn();
    for(int i = 0;i<columns;i++)
        hideColumn(i+firstColumn);
}

void EasyTable::showHiddenRanges()
{
    for(int i = 0;i<RowCount;i++)
    {
        if(isRowHidden(i))
            showRow(i);
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

void EasyTable::useFunction()
{
    functionCode = getFunctionCode();
    switch(functionCode)
    {
    case Cancell :
        return;
    case Count :
        functionCount();
        break;
    case Sum :
        functionSum();
        break;
    case Average :
        functionAverage();
        break;
    default:
        break;
    }
    displayResults();
}

EasyTable::Function EasyTable::getFunctionCode()
{
    QStringList functionItemList;
    functionItemList<<tr("计数")<<tr("求和")<<tr("平均");
    bool ok;
    QString functionItem = QInputDialog::getItem(this,tr("函数"),
                                          tr("请选择函数: "),functionItemList,
                                          0,false,&ok);
    if(ok && !functionItem.isEmpty())
    {
        if(functionItem == tr("计数"))
            return Count;
        if(functionItem == tr("求和"))
            return Sum;
        if(functionItem == tr("平均"))
            return Average;
    }
    return Cancell;
}

void EasyTable::functionCount()
{
    count = 0;
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            Cell *c = cell(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr && !c->text().isEmpty())
                ++count;
        }//end for column
    }//end for row
}

void EasyTable::functionSum()
{
    sum = 0;
    bool ok;
    double temp = 0.0;
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            Cell *c = cell(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr)
            {
                temp = c->text().toDouble(&ok);
                if(ok)
                    sum += temp;
            }
        }//end for column
    }//end for row
}

void EasyTable::functionAverage()
{
    sum = 0,count = 0;
    bool ok;
    double temp = 0.0;
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            Cell *c = cell(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr)
            {
                temp = c->text().toDouble(&ok);
                if(ok)
                {
                    sum += temp;
                    ++count;
                }
            }
        }//end for column
    }//end for row
    if(count != 0)
        average = sum/count;
    else
        average = 0;
}

void EasyTable::displayResults()
{
    switch(functionCode)
    {
    case Count :
        QMessageBox::about(this,tr("计数"),
                           tr("<em>函数结果</em>"
                           "<h1>计数 ： %1 \t</h1>").arg(count));
        break;
    case Sum :
        QMessageBox::about(this,tr("求和"),
                           tr("<em>函数结果</em>"
                           "<h1>求和 ： %1 \t</h1>").arg(sum));
        break;
    case Average :
        QMessageBox::about(this,tr("平均"),
                           tr("<em>函数结果</em>"
                           "<h1>平均 ： %1 \t</h1>").arg(average));
        break;
    default :
        break;
    }
}

void EasyTable::findInAll(const QString &str, Qt::CaseSensitivity cs)
{
    int row = 0;
    int column = 0;
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
        column = 0;
        row++;
    }
    QApplication::beep();
}

void EasyTable::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row = currentRow();
    int column = currentColumn()+1;
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
        column = 0;
        row++;
    }
    QApplication::beep();
}

void EasyTable::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    int row = currentRow();
    int column = currentColumn()-1;
    while(row >= 0)
    {
        while(column >= 0)
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
        column = ColumnCount-1;
        row--;
    }
    QApplication::beep();
}

void EasyTable::recalculate()
{
    for(int column = 0;column<ColumnCount;column++)
    {
        for(int row = 0;row<RowCount;row++)
        {
            if(cell(row,column))
                cell(row,column)->setDirty();
        }//end for
    }//end for
    viewport()->update();
}

void EasyTable::setAutoRecalculate(bool recalc)
{
    autoRecalc = recalc;
    if(autoRecalc)
        recalculate();
}

void EasyTable::setAutoResize(bool resize)
{
    autoResize = resize;
    if(!autoResize)
        disconnect(this,SIGNAL(itemChanged(QTableWidgetItem*)),
                   this,SLOT(resizeCell(QTableWidgetItem*)));
}

void EasyTable::setDefaultAlignment(bool ok)
{
    defaultAlignment = ok;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(ok == false)
    {
        for(int i = 0;i<RowCount;i++)
        {
            for(int j = 0;j<ColumnCount;j++)
            {
                Cell *c = cell(i,j);
                if(c != nullptr)
                    c->setDefaultAlignment(false);
            }//end for column
        }//end for row
    }//end if
    else
    {
        for(int i = 0;i<RowCount;i++)
        {
            for(int j = 0;j<ColumnCount;j++)
            {
                Cell *c = cell(i,j);
                if(c != nullptr)
                    c->setDefaultAlignment(true);
            }//end for column
        }//end for row
    }//end else
    QApplication::restoreOverrideCursor();
}

void EasyTable::sort(const EasyTableCompare &compare, bool defaultChoose)
{
    QTableWidgetSelectionRange range;
    if(defaultChoose)
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        selectAll();
        range = selectedRange();
        columnSort(compare,range);
        QApplication::restoreOverrideCursor();
        somethingChanged();
    }
    else
    {
        range = selectedRange();
        defaultSort(compare,range);
        somethingChanged();
    }
    clearSelection();
}

void EasyTable::columnSort(const EasyTableCompare &compare, const QTableWidgetSelectionRange &range)
//the range is the whole sheet
{
    QList<QStringList> rows;
    int i;
    for(i = 0;i<range.rowCount();i++)
    {
        QStringList row;
        for(int j = 0;j<ColumnCount;j++)
            row.append(formula(range.topRow()+i,
                               range.leftColumn()+j));
        rows.append(row);
    }
    qStableSort(rows.begin(),rows.end(),compare);
    for(i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<ColumnCount;j++)
            setFormula(range.topRow()+i,range.leftColumn()+j,
                       rows[i][j]);
    }
    setCurrentCell(0,0);
}

void EasyTable::defaultSort(const EasyTableCompare &compare, const QTableWidgetSelectionRange &range)
{
    QList<QStringList> rows;
    int i;
    for(i = 0;i<range.rowCount();i++)
    {
        QStringList row;
        for(int j = 0;j<range.columnCount();j++)
            row.append(formula(range.topRow()+i,
                               range.leftColumn()+j));
        rows.append(row);
    }
    qStableSort(rows.begin(),rows.end(),compare);
    for(i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
            setFormula(range.topRow()+i,range.leftColumn()+j,
                       rows[i][j]);
    }
}


void EasyTable::setFont(const QFont &font)
{
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            Cell *c = cell(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr)
                c->setFont(font);
        }//end for column
    }//end for row
}

void EasyTable::setAlignment(int alignment)
//the alignment is from Qt::Align...
{
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            Cell *c = cell(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr)
            {
                c->setDefaultAlignment(false);
                c->setTextAlignment(alignment);
            }
        }//end for column
    }//end for row
}

void EasyTable::setTextColor(QColor &textColor)
{
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            Cell *c = cell(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr)
                c->setTextColor(textColor);
        }//end for column
    }//end for row
}

void EasyTable::setBackgroundColor(QColor &backgroundColor)
{
    QTableWidgetSelectionRange range = selectedRange();
    for(int i = 0;i<range.rowCount();i++)
    {
        for(int j = 0;j<range.columnCount();j++)
        {
            QTableWidgetItem *c = item(range.topRow()+i,range.leftColumn()+j);
            if(c != nullptr)
                c->setBackgroundColor(backgroundColor);
        }//end for column
    }//end for row
}

void EasyTable::setGrid(Qt::PenStyle &gridStyle)
{
    setGridStyle(gridStyle);
}

void EasyTable::cellSizeChange(int row, int column,qreal width,qreal height)
{
     if(width > Width)
        setColumnWidth(column,int(width)+1);
    else
    {
        if(width == 0)
             setColumnWidth(column,Width);
        if(columnWidth(column) > Width)
        {
            int max = Width;
            for(int i = 0;i<RowCount;i++)
            {
                if(columnWidth(i)>Width && columnWidth(i)>max)
                {
                    max = columnWidth(i);
                }//end if
            }//end for
            setColumnWidth(column,max);
        }//end if
    }//end else
    if(height > Height)
        setRowHeight(row,int(height)+1);
    else
    {
        if(rowHeight(row) > Height)
        {
            int max = Height;
            for(int i = 0;i<ColumnCount;i++)
            {
                if(rowHeight(i)>Height && rowHeight(i)>max)
                {
                    max = rowHeight(i);
                }//end if
            }//end for
            setRowHeight(row,max);
        }//end if
    }//end else
}

void EasyTable::getColumnContext(int column, QSet<QString> &strSet, QVector<int> &maxRow)
{
    int row = 0;
    for(int i = 0; i < RowCount; i++)
    {
        Cell *c = cell(i,column);
        if(c != nullptr && !(c->text().isEmpty()))
        {
            strSet.insert(c->text());
            row = i;
        }//end if
    }//end for
    maxRow.append(row);
}

void EasyTable::hideRowUnlike(int column, QString str, int range)
//hide the unlike rows.range is the max unempty row in the whole sheet
{
    QString info;
    for(int i = 0; i <= range; i++)
    {
        Cell *c = cell(i,column);
        if(c != nullptr)
        {
            info = c->text();
            if(info != str)
                hideRow(i);
        }//end if
    }//end for
}
