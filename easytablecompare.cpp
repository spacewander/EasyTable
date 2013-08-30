/**
*@class EasyTableCompare
*store the message for sorting
*it is a functor only used in sort() and its associated functions
*/
/**
*@file
*store the message for sorting
*it is a functor only used in sort() and its associated functions
*/
#include "easytablecompare.h"
/**
*the implement of sort 
*empty row should be put to the bottom of sheet
*/
bool EasyTableCompare::operator ()(const QStringList& row1,
                                     const QStringList& row2) const
{
    for(int i = 0;i<KeyCount;i++)
    {
        int column = keys[i];
        if(column >= 0)
        {
            if(row1[column] != row2[column])
            {
                if(row1[column] != ""  &&  row2[column] != "")
                {
                    if(ascending[i])//if sort by ascending
                    {
                        return smaller(row1[column],row2[column]);
                        //return true if row1[column] < row2[column]
                    }
                    else
                    {
                        return larger(row1[column],row2[column]);
                    }
                }//end if(row1[column] != row2[column])
                else
                    //the empty cell should be at the bottom of unempty cell
                {
                    if(row1[column] == "")
                        return false;
                    else
                        return true;
                }//end else
            }//end if row1[column]  !=  row2[column]
        }//end if column  !=  -1
    }//end for
    return false;
}
/**
*return true if a > b,otherwise return false
*remember the string value is forever ahead of number value
*/
bool EasyTableCompare::larger(const QString &a,const QString &b) const
{
    bool aIsNum = false;
    bool bIsNum = false;
    a.toDouble(&aIsNum);
    b.toDouble(&bIsNum);
    if(!aIsNum && !bIsNum)
        return a > b;
    else if(!aIsNum || !bIsNum)
    {
        return bIsNum == true ;
        //if a is string and b is not,a is larger than b by default.
    }
    else
    {
        double first = a.toDouble();
        double second = b.toDouble();
        return first > second;
    }
    return false;
}
/**
*the same as larger(QString,QString),but return the opposite result
*remember the string value is forever ahead of number value
*/
bool EasyTableCompare::smaller(const QString &a, const QString &b) const
{
    bool aIsNum = false;
    bool bIsNum = false;
    a.toDouble(&aIsNum);
    b.toDouble(&bIsNum);
    if(!aIsNum && !bIsNum)
        return a < b;
    else if(!aIsNum || !bIsNum)
    {
        return bIsNum == true ;
        //if a is string and b is not,a is larger than b by default.
    }
    else
    {
        double first = a.toDouble();
        double second = b.toDouble();
        return first < second;
    }
    return true;
}
