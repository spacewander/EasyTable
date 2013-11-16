/**
 *@file
 * find action controller
 */
/**
 *@class
 * find action controller
 */
#include "find.h"
#include "easytable.h"
#include <QMessageBox>
#include <QApplication>
#include <QRegExp>

/**
 * the constructor of Find
 * @param a EasyTable instance as the base of it and the recevier of it
 */
Find::Find(EasyTable *parent) :
    QObject(parent),parent(parent)
{

}
/**
 * @brief check if the find mediator is able to use
 * @return bool
 */
bool Find::isAble() const
{
    return parent != nullptr;
}

/**
 *the core of find method
 */
void Find::didFind(int row,int column)
{
    parent->clearSelection();
    parent->setCurrentCell(row,column);
    parent->activateWindow();
    emit matched();
}

/**
 *basic find method
 */
bool Find::_find(int row,int column,const QString &str, Qt::CaseSensitivity cs)
{
    if(parent != nullptr)
    {
        if(!parent->text(row,column).isEmpty() && parent->text(row,column).contains(str,cs))
        {
            didFind(row,column);
            return true;
        }
    }
  return false;
}
/**
 *basic find method for using regex
 */
bool Find::_find(int row,int column,const QRegExp &re)
{
    if(parent != nullptr)
    {
        if(!parent->text(row,column).isEmpty() && parent->text(row,column).contains(re))
        {
            didFind(row,column);
            return true;
        }
    }
    return false;
}

/**
*find from the begin of file to the end of file
*/
void Find::findInAll(const QString &str, Qt::CaseSensitivity cs)
{
    int row = 0;
    int column = 0;
    while(row < parent->getRowCount())
    {
        while(column < parent->getColumnCount())
        {
            if(_find(row,column,str,cs) )
                return;
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("查找结束,没有找到"
                                              "<pre>     %1 </pre>").arg(str));
}
/**
*find from here and search the whole file
*/
void Find::findFromHere(const QString &str, Qt::CaseSensitivity cs)
{
    int row = parent->currentRow();
    int column = parent->currentColumn()+1;
    while(row < parent->getRowCount())
    {
        while(column < parent->getColumnCount())
        {
            if(_find(row,column,str,cs) )
                    return;
            column++;
        }
        column = 0;
        row++;
    }
    //go back to search again
    row = 0;
    column = 0;
    while(row < parent->currentRow())
    {
        while(column < parent->getColumnCount() || (row == parent->currentRow() && column <= parent->currentColumn()))
        {
            if(_find(row,column,str,cs) )
                return;
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("查找结束,没有找到"
                                              "<pre>    %1 </pre>").arg(str));
}
/**
*find forward
*/
void Find::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row = parent->currentRow();
    int column = parent->currentColumn()+1;
    while(row < parent->getRowCount())
    {
        while(column < parent->getColumnCount())
        {
            if(_find(row,column,str,cs) )
                return;
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("向后查找结束,没有找到"
                                              "<pre>    %1 </pre>"
                                              "如需继续查找,请使用向前查找").arg(str));
}
/**
*find backward
*/
void Find::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    int row = parent->currentRow();
    int column = parent->currentColumn()-1;
    while(row >= 0)
    {
        while(column >= 0)
        {
            if(_find(row,column,str,cs) )
                return;
            column--;
        }
        column = parent->getColumnCount() - 1;
        row--;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("向前查找结束,没有找到"
                                              "<pre>    %1 </pre>"
                                              "如需继续查找,请使用向后查找").arg(str));
}

/**
*find from the begin of file to the end of file
*using regex
*/
void Find::findInAll(const QRegExp &re)
{
    int row = 0;
    int column = 0;
    while(row < parent->getRowCount())
    {
        while(column < parent->getColumnCount())
        {
            if(_find(row,column,re) )
                return;
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("查找结束,没有找到"
                                                "%1").arg(re.errorString() ));
}
/**
*find from here and search the whole file
*using regex
*/
void Find::findFromHere(const QRegExp &re)
{
    int row = parent->currentRow();
    int column = parent->currentColumn()+1;
    while(row < parent->getRowCount())
    {
        while(column < parent->getColumnCount())
        {
            if(_find(row,column,re) )
                    return;
            column++;
        }
        column = 0;
        row++;
    }
    //go back to search again
    row = 0;
    column = 0;
    while(row < parent->currentRow())
    {
        while(column < parent->getColumnCount() || (row == parent->currentRow() && column <= parent->currentColumn()))
        {
            if(_find(row,column,re) )
                return;
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("查找结束,没有找到"
                                              "%1").arg(re.errorString() ));
}
/**
*find forward
*using regex
*/
void Find::findNext(const QRegExp &re)
{
    int row = parent->currentRow();
    int column = parent->currentColumn()+1;
    while(row < parent->getRowCount())
    {
        while(column < parent->getColumnCount())
        {
            if(_find(row,column,re) )
                return;
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("向后查找结束,没有找到"
                                              "%1"
                                              "如需继续查找,请使用向前查找").arg(re.errorString() ));
}
/**
*find backward
*using regex
*/
void Find::findPrevious(const QRegExp &re)
{
    int row = parent->currentRow();
    int column = parent->currentColumn()-1;
    while(row >= 0)
    {
        while(column >= 0)
        {
            if(_find(row,column,re) )
                return;
            column--;
        }
        column = parent->getColumnCount() - 1;
        row--;
    }
    QApplication::beep();
    QMessageBox::information(parent,tr("查找"),tr("向前查找结束,没有找到"
                                              "%1"
                                              "如需继续查找,请使用向后查找").arg(re.errorString() ));
}

/**
*replace the context of the cell found
*/
void Find::replaceSelectedCell(const QString &str)
{
    parent->finish(str);
}
