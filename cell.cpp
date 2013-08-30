/**
*@class Cell
*a special tableWidgetItem for EasyTable
*/
/**
*@file
*a special tableWidgetItem for EasyTable
*/
#include "cell.h"
/**
*the constructor of Cell
*/
Cell::Cell() :
    QTableWidgetItem(QTableWidgetItem::Type)
{
    setDirty();
}
/**
*clone a new Cell with everything the same 
*/
QTableWidgetItem* Cell::clone() const
{
    return new Cell(*this);
}
/**
*set formula
*/
void Cell::setFormula(const QString &formula)
{
    setData(Qt::EditRole,formula);
}
/**
*return text with the cell`s formula
*/
QString Cell::formula() const
{
    return data(Qt::EditRole).toString();
}
/**
*write into the Cell
*/
void Cell::setData(int role, const QVariant &value)
{
    QTableWidgetItem::setData(role,value);
    if(Qt::EditRole == role)
    {
        setDirty();
        defaultAlignment = true;
    }
}
/**
*set the value of Cell has been changed
*/
void Cell::setDirty()
{
    cacheIsDirty = true;
}
/**
*set whether default alignment is used
*/
void Cell::setDefaultAlignment(bool ok)
{
    defaultAlignment = ok;
}
/**
*return value of a cell depends the role of cell(edit or display)
*/
QVariant Cell::data(int role) const
{
    if(Qt::DisplayRole == role)
    {
        if(value().isValid())
        {
            return value().toString();
        }
        else
        {
            return "????";
        }
    }// end if
    else if(Qt::TextAlignmentRole  == role && defaultAlignment == true)
    {
        if(QVariant::String == value().type())
        {
            return int(Qt::AlignLeft|Qt::AlignVCenter);
        }
        else
        {
            return int(Qt::AlignRight|Qt::AlignVCenter);
        }
    }//end else if
    else
        return QTableWidgetItem::data(role);
}
/// define a Null object
const QVariant INVALID;

/**
*return value of a cell
*and calculate the value if the cell stores a formula
*The formula I use here means an equation combines with basic operator like *+-/
*/
QVariant Cell::value() const
{
    if(cacheIsDirty)
    {
        cacheIsDirty = false;
        QString formulaStr = formula();
        if(formulaStr.startsWith('\''))
        {
            cachedValue = formulaStr.mid(1);
        }
        else if(formulaStr.startsWith('='))
        {
            cachedValue = INVALID;
            QString expr = formulaStr.mid(1);
            expr.replace(" ","");
            expr.append(QChar::Null);
            int pos = 0;
            cachedValue = evalExpression(expr,pos);
            if(expr[pos]  !=  QChar::Null)
                cachedValue = INVALID;
        }
        else
        {
            bool ok;
            double d = formulaStr.toDouble(&ok);
            if(ok)
                cachedValue = d;
            else
                cachedValue = formulaStr;
        }//end else
    }//end if cacheIsDirty
    return cachedValue;
}
/**
*handle formula(equation)
*deal with +- itself
*/
QVariant Cell::evalExpression(const QString &str, int &pos) const
{
    QVariant result = evalTerm(str,pos);
    while(str[pos]  !=  QChar::Null)
    {
        QChar op = str[pos];
        if(op != '+'&&op != '-')
            return result;
        pos++;
        QVariant term = evalTerm(str,pos);
        if(QVariant::Double == result.type()&&
                QVariant::Double == term.type())
        {
            if('+' == op)
                result = result.toDouble()+term.toDouble();
            else
                result = result.toDouble()-term.toDouble();
        }
        else
        {
            result = INVALID;
        }//end else
    }//end while
    return result;
}
/**
*handle formula(equation)
*deal with /* itself
*/
QVariant Cell::evalTerm(const QString &str, int &pos) const
{
    QVariant result = evalFactor(str,pos);
    while(str[pos] != QChar::Null)
    {
        QChar op = str[pos];
        if(op != '*'&&op != '/')
            return result;
        pos++;
        QVariant factor = evalFactor(str,pos);
        if(QVariant::Double == result.type()&&
                QVariant::Double == factor.type())
        {
            if('*' == op)
                result = result.toDouble()*factor.toDouble();
            else
            {
                if(factor.toDouble() != 0.0)
                    result = result.toDouble()/factor.toDouble();
                else
                    result = INVALID;
            }//end else
        }//end if
        else
            result = INVALID;
    }//end while
    return result;
}
/**
*handle formula(equation)
*deal with negative or positive itself and call other function to deal with other cases
*/
QVariant Cell::evalFactor(const QString &str, int &pos) const
{
    QVariant result;
    bool negative = false;
    if('-' == str[pos])
    {
        negative = true;
        pos++;
    }
    if('(' == str[pos])
    {
        pos++;
        result = evalExpression(str,pos);
        if(str[pos] != ')')
            result = INVALID;
        pos++;
    }
    else
    {
        QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
        QString token;
        while(str[pos].isLetterOrNumber()||'.' == str[pos])
        {
            token += str[pos];
            pos++;
        }
        if(regExp.exactMatch(token))
        {
            int column = token[0].toUpper().unicode()-'A';
            int row = token.mid(1).toInt()-1;
            Cell *c = static_cast<Cell* >(tableWidget()->
                                        item(row,column));
            if(c)
                result = c->value();
            else
                result = 0.0;
        }
        else
        {
            bool ok;
            result = token.toDouble(&ok);
            if(!ok)
                result = INVALID;
        }//end else for !regExp.exactMatch(token)
    }//end else for '(' != str[pos]
    if(negative)
    {
        if(QVariant::Double == result.type())
            result = -result.toDouble();
        else
            result = INVALID;
    }//end if
    return result;
}