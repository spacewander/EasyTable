#include "easytablecompare.h"

bool EasyTableCompare::operator ()(const QStringList& row1,
                                     const QStringList& row2) const
{
    for(int i = 0;i<KeyCount;i++)
    {
        int column = keys[i];
        if(column != -1)
        {
            if(row1[column] != row2[column])
            {
                if(row1[column] != "" && row2[column] != "")
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

bool EasyTableCompare::larger(const QString &a,const QString &b) const
//return true if a > b,otherwise return false
{
    bool boolForA = false;
    bool boolForB = false;
    a.toDouble(&boolForA);
    a.toDouble(&boolForB);
    if(!boolForA && !boolForB)
        return a > b;
    else if(!boolForA || !boolForB)
    {
        return boolForA == false ? true : false;
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

bool EasyTableCompare::smaller(const QString &a, const QString &b) const
//the same as larger(QString,QString),but return the opposite result
{
    bool boolForA = false;
    bool boolForB = false;
    a.toDouble(&boolForA);
    a.toDouble(&boolForB);
    if(!boolForA && !boolForB)
        return a < b;
    else if(!boolForA || !boolForB)
    {
        return boolForA == true ? true : false;
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
